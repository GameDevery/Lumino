
#include "../Internal.h"
#include <ft2build.h>
#include FT_IMAGE_H
#include FT_OUTLINE_H
#include "FreeTypeFontTessellator.h"

LN_NAMESPACE_BEGIN
namespace detail {



	Vector2 Filled::FTVectorToLNVector(const FT_Vector* ftVec)
	{
		return Vector2(
			(double)(ftVec->x / 64) + (double)(ftVec->x % 64) / 64.,
			(double)(ftVec->y / 64) + (double)(ftVec->y % 64) / 64.);
	}

	void Filled::Initialize()
	{
		m_tessellationStep = 3;

		m_ftOutlineFuncs.move_to = (FT_Outline_MoveTo_Func)ftMoveToCallback;
		m_ftOutlineFuncs.line_to = (FT_Outline_LineTo_Func)ftLineToCallback;
		m_ftOutlineFuncs.conic_to = (FT_Outline_ConicTo_Func)ftConicToCallback;
		m_ftOutlineFuncs.cubic_to = (FT_Outline_CubicTo_Func)ftCubicToCallback;
		m_ftOutlineFuncs.shift = 0;
		m_ftOutlineFuncs.delta = 0;

		m_gluTesselator = gluNewTess();

		// �f�t�H���g�� GLU_TESS_WINDING_ODD.
		// �����A���ꂾ�Ɠʖʂ��d�Ȃ�悤�ȃO���t�őz��O�̌���������������B(���C���I��"��" ���킩��₷��)
		// �Ƃ肠���� GLU_TESS_WINDING_NONZERO �őS�����߂�悤�ɂ���B
		gluTessProperty(m_gluTesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

		gluTessCallback(m_gluTesselator, GLU_TESS_BEGIN_DATA, (GLUTessCallback)tessBeginCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_END_DATA, (GLUTessCallback)tessEndCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_VERTEX_DATA, (GLUTessCallback)vertexDataCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_COMBINE_DATA, (GLUTessCallback)combineCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_ERROR, (GLUTessCallback)errorCallback);
	}

	Filled::~Filled(void)
	{
		gluDeleteTess(m_gluTesselator);
	}


void Filled::DecomposeOutlineVertices(FreeTypeFont* font, UTF32 utf32code)
{
	font->UpdateFont();
	FT_Face face = font->GetFTFace();

	//FT_OutlineGlyph g;
	//FT_Error error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&g);

	FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		font->GetManager()->GetFTCacheMapCache(),
		font->GetFTCFaceId(),
		font->GetFTCacheMapIndex(),
		utf32code);

	//FT_Glyph glyph;
	FT_OutlineGlyph g;
	FTC_ImageTypeRec imageType = {};
	imageType.face_id = font->GetFTCFaceId();
	imageType.height = font->GetSize();
	imageType.width = 0;
	imageType.flags = 0;
	FT_Error error = FTC_ImageCache_Lookup(
		font->GetManager()->GetFTCImageCache(),
		&imageType,
		glyphIndex,
		(FT_Glyph*)&g,
		NULL);


	m_vectorScale = 0.15f;//(m_pointSize * m_resolution) / (72. * face->units_per_EM);



	error = FT_Outline_Decompose(&g->outline, &m_ftOutlineFuncs, this);

	//FT_Done_Glyph((FT_Glyph)g);


	// 1�O�� Outline ������΁A���_�����m�肳����
	if (!m_contourOutlineList.IsEmpty())
	{
		ContourOutline& outline = m_contourOutlineList.GetLast();
		outline.indexCount = m_vertexList.GetCount() - outline.startIndex;
	}

	CalculateExtrusion();
}


// �|�C���g���ړ����邾���B�܂��_�͒u���Ȃ��B
int Filled::ftMoveToCallback(FT_Vector* to, Filled* thisData)
{
	// 1�O�� Outline ������΁A���_�����m�肳����
	if (!thisData->m_contourOutlineList.IsEmpty())
	{
		ContourOutline& outline = thisData->m_contourOutlineList.GetLast();
		outline.indexCount = thisData->m_vertexList.GetCount() - outline.startIndex;
	}

	// �V���� Contour ���J�n����
	ContourOutline contour;
	contour.startIndex = thisData->m_vertexList.GetCount();
	thisData->m_contourOutlineList.Add(contour);

	thisData->m_lastVertex = FTVectorToLNVector(to);
	//printf("moveto ");
	//thisData->m_lastVertex.Print();
	return 0;
}

int Filled::ftLineToCallback(FT_Vector* to, Filled* thisData)
{
	Vector2 v = FTVectorToLNVector(to);

	thisData->m_vertexList.Add(v * thisData->m_vectorScale);

	thisData->m_lastVertex = v;
	//thisData->m_lastVertex.Print();
	return 0;
}

int Filled::ftConicToCallback(FT_Vector* control, FT_Vector* to, Filled* thisData)
{
	Vector2 to_vertex = FTVectorToLNVector(to);
	Vector2 control_vertex = FTVectorToLNVector(control);

	double b[2], c[2], d[2], f[2], df[2], d2f[2];

	b[X] = thisData->m_lastVertex.x - 2 * control_vertex.x +
		to_vertex.x;
	b[Y] = thisData->m_lastVertex.y - 2 * control_vertex.y +
		to_vertex.y;

	c[X] = -2 * thisData->m_lastVertex.x + 2 * control_vertex.x;
	c[Y] = -2 * thisData->m_lastVertex.y + 2 * control_vertex.y;

	d[X] = thisData->m_lastVertex.x;
	d[Y] = thisData->m_lastVertex.y;

	f[X] = d[X];
	f[Y] = d[Y];
	df[X] = c[X] * thisData->m_delta1 + b[X] * thisData->m_delta2;
	df[Y] = c[Y] * thisData->m_delta1 + b[Y] * thisData->m_delta2;
	d2f[X] = 2 * b[X] * thisData->m_delta2;
	d2f[Y] = 2 * b[Y] * thisData->m_delta2;

	for (unsigned int i = 0; i < thisData->m_tessellationStep - 1; i++)
	{
		f[X] += df[X];
		f[Y] += df[Y];

		Vector2 v(f[0], f[1]);
		v *= thisData->m_vectorScale;
		thisData->m_vertexList.Add(v);

		df[X] += d2f[X];
		df[Y] += d2f[Y];
	}

	Vector2 v = FTVectorToLNVector(to);
	v *= thisData->m_vectorScale;
	thisData->m_vertexList.Add(v);

	thisData->m_lastVertex = to_vertex;

	return 0;
}

int Filled::ftCubicToCallback(FT_Vector* control1, FT_Vector* control2, FT_Vector* to, Filled* thisData)
{
	Vector2 to_vertex = FTVectorToLNVector(to);
	Vector2 control1_vertex = FTVectorToLNVector(control1);
	Vector2 control2_vertex = FTVectorToLNVector(control2);

	double a[2], b[2], c[2], d[2], f[2], df[2], d2f[2], d3f[2];

	a[X] = -thisData->m_lastVertex.x + 3 * control1_vertex.x
		- 3 * control2_vertex.x + to_vertex.x;
	a[Y] = -thisData->m_lastVertex.y + 3 * control1_vertex.y
		- 3 * control2_vertex.y + to_vertex.y;

	b[X] = 3 * thisData->m_lastVertex.x - 6 * control1_vertex.x +
		3 * control2_vertex.x;
	b[Y] = 3 * thisData->m_lastVertex.y - 6 * control1_vertex.y +
		3 * control2_vertex.y;

	c[X] = -3 * thisData->m_lastVertex.x + 3 * control1_vertex.x;
	c[Y] = -3 * thisData->m_lastVertex.y + 3 * control1_vertex.y;

	d[X] = thisData->m_lastVertex.x;
	d[Y] = thisData->m_lastVertex.y;

	f[X] = d[X];
	f[Y] = d[Y];
	df[X] = c[X] * thisData->m_delta1 + b[X] * thisData->m_delta2
		+ a[X] * thisData->m_delta3;
	df[Y] = c[Y] * thisData->m_delta1 + b[Y] * thisData->m_delta2
		+ a[Y] * thisData->m_delta3;
	d2f[X] = 2 * b[X] * thisData->m_delta2 + 6 * a[X] * thisData->m_delta3;
	d2f[Y] = 2 * b[Y] * thisData->m_delta2 + 6 * a[Y] * thisData->m_delta3;
	d3f[X] = 6 * a[X] * thisData->m_delta3;
	d3f[Y] = 6 * a[Y] * thisData->m_delta3;

	for (unsigned int i = 0; i < thisData->m_tessellationStep - 1; i++)
	{
		f[X] += df[X];
		f[Y] += df[Y];

		Vector2 v(f[0], f[1]);
		v *= thisData->m_vectorScale;
		thisData->m_vertexList.Add(v);

		df[X] += d2f[X];
		df[Y] += d2f[Y];
		d2f[X] += d3f[X];
		d2f[Y] += d3f[Y];
	}

	Vector2 v = FTVectorToLNVector(to);
	v *= thisData->m_vectorScale;
	thisData->m_vertexList.Add(v);

	thisData->m_lastVertex = to_vertex;

	return 0;
}


void Filled::CalculateExtrusion()
{
	for (const ContourOutline& outline : m_contourOutlineList)
	{
		int end = outline.startIndex + outline.indexCount;
		for (int i = outline.startIndex; i < end; i++)
		{
			int iPrev = i - 1;
			if (iPrev < outline.startIndex) iPrev = end - 1;
			int iNext = i + 1;
			if (iNext >= end) iNext = outline.startIndex;

			auto& cur = m_vertexList[i];
			auto& prev = m_vertexList[iPrev];
			auto& next = m_vertexList[iNext];

			Vector2 d0 = Vector2::Normalize(cur.pos - prev.pos);//cur.pos - prev.pos;//
			Vector2 d1 = Vector2::Normalize(next.pos - cur.pos);//next.pos - cur.pos;//
			//Vector2 v = prev.pos.x * next.pos.y - next.pos.x * prev.pos.y;
			//Vector2::clo
			float dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
			dlx0 = d0.y;//p0->dy;
			dly0 = -d0.x;
			dlx1 = d1.y;
			dly1 = -d1.x;
			// Calculate extrusions
			// �i�s�����̍���������
			cur.extrusion.x = -(dlx0 + dlx1) * 0.5f;
			cur.extrusion.y = -(dly0 + dly1) * 0.5f;
			cur.extrusion.Normalize();
		}
	}
}

void Filled::Tessellate()
{
	::gluTessBeginPolygon(m_gluTesselator, this);

	for (const ContourOutline& outline : m_contourOutlineList)
	{
		::gluTessBeginContour(m_gluTesselator);

		for (int i = 0; i < outline.indexCount; i++)
		{
			int vertexIndex = outline.startIndex + i;
			const Vector2& v = m_vertexList[vertexIndex].pos;
			GLfloat coords[3] = {v.x, v.y, 0};
			::gluTessVertex(m_gluTesselator, coords, reinterpret_cast<void*>(vertexIndex));
		}

		::gluTessEndContour(m_gluTesselator);
	}

	::gluTessEndPolygon(m_gluTesselator);
}


void Filled::MakeEdgeStroke()
{
	for (const ContourOutline& outline : m_contourOutlineList)
	{
		int end = outline.startIndex + outline.indexCount;

		float extRate = 0.075;

		int i = outline.startIndex;
		auto& cur = m_vertexList[i];

		int iNextExt = 0;
		int iStartExt = m_vertexList.GetCount();
		int iCurExt = m_vertexList.GetCount();
		m_vertexList.Add(cur.pos + cur.extrusion * extRate);
		m_vertexList.GetLast().alpha = 0.0f;



		for (int i = outline.startIndex; i < end; i++)
		{
			//int iPrev = i - 1;
			//if (iPrev < outline.startIndex) iPrev = end - 1;
			int iNext = i + 1;
			if (iNext >= end) iNext = outline.startIndex;




			auto& next = m_vertexList[iNext];

			if (iNext != outline.startIndex)	// start �͉����o���ς�
			{
				// next �̂������o��
				iNextExt = m_vertexList.GetCount();
				m_vertexList.Add(next.pos + next.extrusion * extRate);
				m_vertexList.GetLast().alpha = 0.0f;
			}
			else
			{
				iNextExt = iStartExt;
			}

			auto& cur = m_vertexList[i];


			int i0 = i;
			int i1 = iNext;
			int i2 = iCurExt;
			int i3 = iNextExt;

			m_triangleIndexList.Add(i0);
			m_triangleIndexList.Add(i1);
			m_triangleIndexList.Add(i2);

			m_triangleIndexList.Add(i2);
			m_triangleIndexList.Add(i1);
			m_triangleIndexList.Add(i3);
			//Vector2 p0 = cur.pos;
			//Vector2 p1 = next.pos;
			//Vector2 p2 = m_vertexList[iCurExt].pos;

			iCurExt = iNextExt;
		}


		//return;
	}
}

void Filled::tessBeginCallback(GLenum primitiveType, Filled* thisData)
{
	Contour c;
	c.primitiveType = primitiveType;
	c.intermediateVertexIndex1 = -1;
	c.intermediateVertexIndex2 = -1;
	c.faceCount = 0;
	thisData->m_contourList.Add(c);
}

void Filled::tessEndCallback(Filled* thisData)
{
}

// vertex : gluTessVertex() �� data
// �_�͎��v���ő����Ă���悤���B
void Filled::vertexDataCallback(void* vertexData, Filled* thisData)
{
	Contour* contour = &thisData->m_contourList.GetLast();
	int vertexIndex = reinterpret_cast<int>(vertexData);

	switch (contour->primitiveType)
	{
		// �ʏ�̎O�p�`���X�g�B3���_�����ɑ����Ă���B
		case GL_TRIANGLES:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				// 1�߂̓_
				contour->intermediateVertexIndex1 = vertexIndex;
				contour->intermediateVertexIndex2 = -1;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				// 2�߂̓_
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				// 3�̓_���ł���������
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
				thisData->m_triangleIndexList.Add(vertexIndex);
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex1 = -1;
			}
			break;
		}
		// TriangleStrip �͍Ō��2�_���ė��p����B
		case GL_TRIANGLE_STRIP:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				contour->intermediateVertexIndex1 = vertexIndex;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
#if 1

				if (contour->faceCount & 1)	// ���
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
					thisData->m_triangleIndexList.Add(vertexIndex);
				}
				else	// ������ (����܂�)
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(vertexIndex);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				}

				contour->intermediateVertexIndex1 = contour->intermediateVertexIndex2;
				contour->intermediateVertexIndex2 = vertexIndex;
				contour->faceCount++;
#else
				if (thisData->m_triangleIndexList.GetCount() & 1)	// ���
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(vertexIndex);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				}
				else	// ������ (����܂�)
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
					thisData->m_triangleIndexList.Add(vertexIndex);


					thisData->m_vertexList[contour->intermediateVertexIndex1].Print();
					thisData->m_vertexList[contour->intermediateVertexIndex2].Print();
					thisData->m_vertexList[vertexIndex].Print();

				}
				contour->intermediateVertexIndex1 = contour->intermediateVertexIndex2;
				contour->intermediateVertexIndex2 = vertexIndex;
#endif
			}
			break;
		}
		// TriangleFan �́A�ŏ��̓_�������ƍė��p����B
		case GL_TRIANGLE_FAN:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				contour->intermediateVertexIndex1 = vertexIndex;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
				thisData->m_triangleIndexList.Add(vertexIndex);
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			break;
		}
	}
}

void Filled::combineCallback(GLfloat coords[3], void* vertex_data[4], GLfloat weight[4], void** out_data, Filled* thisData)
{
	(void)vertex_data;
	(void)weight;
	//    std::cerr << "called combine" << std::endl;
	//VertexInfo* vertex = new VertexInfo(coords);
	//*out_data = vertex;
	//filled->extraVertices().push_back(vertex);
	//LN_ASSERT(0);

	VertexInfo v(Vector2(coords[0], coords[1]));
	thisData->m_vertexList.Add(v);
	*out_data = reinterpret_cast<void*>(thisData->m_vertexList.GetCount()-1);

	//int vertexIndex = outline.startIndex + i;
	//const Vector2& v = m_vertexList[vertexIndex].pos;
	//GLfloat coords[3] = { v.x, v.y, 0 };
	//::gluTessVertex(m_gluTesselator, coords, reinterpret_cast<void*>(vertexIndex));
}

void Filled::errorCallback(GLenum error_code)
{
	//std::cerr << "hmm. error during tessellation?:" << gluErrorString(error_code) << std::endl;
}






#if 0
	void Filled::renderGlyph(FreeTypeFont* font, UTF32 utf32code)
	{
		//FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

		//if (error != 0)
		//	return;

		//FT_OutlineGlyph g;

		//error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&g);

		//if (error != 0)
		//	return;



		//FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		//	font->GetManager()->GetFTCacheMapCache(),
		//	m_ftFaceID,
		//	m_ftCacheMapIndex,
		//	utf32code);

		//FT_Glyph glyph;
		//FT_Error err = FTC_ImageCache_Lookup(
		//	m_manager->GetFTCImageCache(),
		//	&m_ftImageType,
		//	glyphIndex,
		//	&glyph,
		//	NULL);

		font->UpdateFont();
		FT_Face face = font->GetFTFace();

		//FT_OutlineGlyph g;
		//FT_Error error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&g);
		
		FT_UInt glyphIndex = FTC_CMapCache_Lookup(
			font->GetManager()->GetFTCacheMapCache(),
			font->GetFTCFaceId(),
			font->GetFTCacheMapIndex(),
			utf32code);

		//FT_Glyph glyph;
		FT_OutlineGlyph g;
		FTC_ImageTypeRec imageType = {};
		imageType.face_id = font->GetFTCFaceId();
		imageType.height = 12;
		imageType.width = 0;
		imageType.flags = 0;
		FT_Error error = FTC_ImageCache_Lookup(
			font->GetManager()->GetFTCImageCache(),
			&imageType,
			glyphIndex,
			(FT_Glyph*)&g,
			NULL);


		//FT_OutlineGlyph g;
		//FT_Error error = FT_Get_Glyph(glyph, (FT_Glyph*)&g);







		m_vectorScale = (m_pointSize * m_resolution) / (72. * face->units_per_EM);

		//if (character_rotation_.active_) {
		//	glPushMatrix();
		//	glTranslatef((face->glyph->metrics.width / 2. +
		//		face->glyph->metrics.horiBearingX) / 64.
		//		* m_vectorScale,
		//		rotation_offset_y_,
		//		0.);

		//	if (character_rotation_.x_ != 0.)
		//		glRotatef(character_rotation_.x_, 1., 0., 0.);

		//	if (character_rotation_.y_ != 0.)
		//		glRotatef(character_rotation_.y_, 0., 1., 0.);

		//	if (character_rotation_.z_ != 0.)
		//		glRotatef(character_rotation_.z_, 0., 0., 1.);

		//	glTranslatef(-(face->glyph->metrics.width / 2. +
		//		face->glyph->metrics.horiBearingX) / 64.
		//		* m_vectorScale,
		//		-rotation_offset_y_,
		//		0.);
		//}

		//if (depth_offset_ != 0.) {
		//	glPushMatrix();
		//	glTranslatef(0., 0., depth_offset_);
		//	glNormal3f(0., 0., 1.);
		//}
		//else {
		//	glNormal3f(0., 0., -1.);
		//}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		contour_open_ = false;

		gluTessBeginPolygon(m_gluTesselator, this);

		// The Big Kahuna: the FreeType glyph decomposition routine traverses
		// the outlines of the font by calling the various routines stored in
		// m_ftOutlineFuncs. These routines in turn call the GLU tessellation routines
		// to create OGL polygons.

		error = FT_Outline_Decompose(&g->outline, &m_ftOutlineFuncs, this);

		FT_Done_Glyph((FT_Glyph)g);

		// Some glyphs may be empty (the 'blank' for instance!)

		if (contour_open_)
			gluTessEndContour(m_gluTesselator);

		gluTessEndPolygon(m_gluTesselator);

		//if (depth_offset_ != 0.) {
		//	glPopMatrix();
		//}
		//if (character_rotation_.active_) {
		//	glPopMatrix();
		//}

		// Drawing a character always advances the MODELVIEW.

		//glTranslatef(face->glyph->advance.x / 64 * m_vectorScale,
		//	face->glyph->advance.y / 64 * m_vectorScale,
		//	0.);

		//for (VILI vili = extra_vertices_.begin(); vili != extra_vertices_.end(); vili++)
		//	delete *vili;

		//extra_vertices_.clear();

		//for (VILI vili = vertices_.begin(); vili != vertices_.end(); vili++)
		//	delete *vili;

		//vertices_.clear();
	}

	// �|�C���g���ړ����邾���B�܂��_�͒u���Ȃ��B
	int Filled::ftMoveToCallback(FT_Vector* to, Filled* filled)
	{
		if (filled->contour_open_) {
			gluTessEndContour(filled->m_gluTesselator);
		}

		filled->m_lastVertex = FTVectorToLNVector(to);

		gluTessBeginContour(filled->m_gluTesselator);

		filled->contour_open_ = true;

		return 0;
	}

	int Filled::ftLineToCallback(FT_Vector* to, Filled* filled)
	{
		filled->m_lastVertex = FTVectorToLNVector(to);

		Vector3 vertex = filled->m_lastVertex;
		vertex *= filled->m_vectorScale;

		//VertexInfo* vertex = new VertexInfo(to, filled->colorTess(), filled->textureTess());

		////float y = 
		//vertex->v_[X] *= filled->m_vectorScale;
		//vertex->v_[Y] *= filled->m_vectorScale;

		gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, (void*)1);//nullptr/*vertex*/);

		//filled->vertices_.push_back(vertex);

		return 0;
	}

	int Filled::ftConicToCallback(FT_Vector* control, FT_Vector* to, Filled* filled)
	{
		// This is crude: Step off conics with a fixed number of increments

		//VertexInfo to_vertex(to, filled->colorTess(), filled->textureTess());
		//VertexInfo control_vertex(control, filled->colorTess(), filled->textureTess());
		Vector3 to_vertex = FTVectorToLNVector(to);
		Vector3 control_vertex = FTVectorToLNVector(control);

		double b[2], c[2], d[2], f[2], df[2], d2f[2];

		b[X] = filled->m_lastVertex.x - 2 * control_vertex.x +
			to_vertex.x;
		b[Y] = filled->m_lastVertex.y - 2 * control_vertex.y +
			to_vertex.y;

		c[X] = -2 * filled->m_lastVertex.x + 2 * control_vertex.x;
		c[Y] = -2 * filled->m_lastVertex.y + 2 * control_vertex.y;

		d[X] = filled->m_lastVertex.x;
		d[Y] = filled->m_lastVertex.y;

		f[X] = d[X];
		f[Y] = d[Y];
		df[X] = c[X] * filled->m_delta1 + b[X] * filled->m_delta2;
		df[Y] = c[Y] * filled->m_delta1 + b[Y] * filled->m_delta2;
		d2f[X] = 2 * b[X] * filled->m_delta2;
		d2f[Y] = 2 * b[Y] * filled->m_delta2;

		for (unsigned int i = 0; i < filled->m_tessellationStep - 1; i++) {

			f[X] += df[X];
			f[Y] += df[Y];

			//VertexInfo* vertex = new VertexInfo(f, filled->colorTess(), filled->textureTess());

			//vertex->v_[X] *= filled->m_vectorScale;
			//vertex->v_[Y] *= filled->m_vectorScale;

			////filled->vertices_.push_back(vertex);

			//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);


			Vector3 vertex(f[0], f[1], 0);
			vertex *= filled->m_vectorScale;
			gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);


			df[X] += d2f[X];
			df[Y] += d2f[Y];
		}

		//VertexInfo* vertex = new VertexInfo(to, filled->colorTess(), filled->textureTess());

		//vertex->v_[X] *= filled->m_vectorScale;
		//vertex->v_[Y] *= filled->m_vectorScale;

		////filled->vertices_.push_back(vertex);

		//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);
		Vector3 vertex = FTVectorToLNVector(to);
		vertex *= filled->m_vectorScale;
		gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);

		filled->m_lastVertex = to_vertex;

		return 0;
	}

	int Filled::ftCubicToCallback(FT_Vector* control1, FT_Vector* control2,
		FT_Vector* to, Filled* filled)
	{
		// This is crude: Step off cubics with a fixed number of increments

		//VertexInfo to_vertex(to, filled->colorTess(), filled->textureTess());
		//VertexInfo control1_vertex(control1, filled->colorTess(), filled->textureTess());
		//VertexInfo control2_vertex(control2, filled->colorTess(), filled->textureTess());

		Vector3 to_vertex = FTVectorToLNVector(to);
		Vector3 control1_vertex = FTVectorToLNVector(control1);
		Vector3 control2_vertex = FTVectorToLNVector(control2);

		double a[2], b[2], c[2], d[2], f[2], df[2], d2f[2], d3f[2];

		a[X] = -filled->m_lastVertex.x + 3 * control1_vertex.x
			- 3 * control2_vertex.x + to_vertex.x;
		a[Y] = -filled->m_lastVertex.y + 3 * control1_vertex.y
			- 3 * control2_vertex.y + to_vertex.y;

		b[X] = 3 * filled->m_lastVertex.x - 6 * control1_vertex.x +
			3 * control2_vertex.x;
		b[Y] = 3 * filled->m_lastVertex.y - 6 * control1_vertex.y +
			3 * control2_vertex.y;

		c[X] = -3 * filled->m_lastVertex.x + 3 * control1_vertex.x;
		c[Y] = -3 * filled->m_lastVertex.y + 3 * control1_vertex.y;

		d[X] = filled->m_lastVertex.x;
		d[Y] = filled->m_lastVertex.y;

		f[X] = d[X];
		f[Y] = d[Y];
		df[X] = c[X] * filled->m_delta1 + b[X] * filled->m_delta2
			+ a[X] * filled->m_delta3;
		df[Y] = c[Y] * filled->m_delta1 + b[Y] * filled->m_delta2
			+ a[Y] * filled->m_delta3;
		d2f[X] = 2 * b[X] * filled->m_delta2 + 6 * a[X] * filled->m_delta3;
		d2f[Y] = 2 * b[Y] * filled->m_delta2 + 6 * a[Y] * filled->m_delta3;
		d3f[X] = 6 * a[X] * filled->m_delta3;
		d3f[Y] = 6 * a[Y] * filled->m_delta3;

		for (unsigned int i = 0; i < filled->m_tessellationStep - 1; i++) {

			f[X] += df[X];
			f[Y] += df[Y];

			//VertexInfo* vertex = new VertexInfo(f, filled->colorTess(), filled->textureTess());

			//vertex->v_[X] *= filled->m_vectorScale;
			//vertex->v_[Y] *= filled->m_vectorScale;

			////filled->vertices_.push_back(vertex);

			//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);
			Vector3 vertex(f[0], f[1], 0);
			vertex *= filled->m_vectorScale;
			gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);

			df[X] += d2f[X];
			df[Y] += d2f[Y];
			d2f[X] += d3f[X];
			d2f[Y] += d3f[Y];
		}

		//VertexInfo* vertex = new VertexInfo(to, filled->colorTess(), filled->textureTess());

		//vertex->v_[X] *= filled->m_vectorScale;
		//vertex->v_[Y] *= filled->m_vectorScale;

		////filled->vertices_.push_back(vertex);

		//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);
		Vector3 vertex = FTVectorToLNVector(to);
		vertex *= filled->m_vectorScale;
		gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);

		filled->m_lastVertex = to_vertex;

		return 0;
	}

	void Filled::beginCallback(GLenum primitiveType)
	{
		printf("");
		//GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, or GL_TRIANGLES
		
	}

	void Filled::endCallback()
	{

	}

	// vertex : gluTessVertex() �� data
	void Filled::vertexDataCallback(VertexInfo* vertex, void* userData)
	{
		//glVertex3dv(vertex->v_);
		vertex->Print();
	}

	void Filled::combineCallback(GLdouble coords[3], void* vertex_data[4],
		GLfloat weight[4], void** out_data,
		Filled* filled)
	{
		(void)vertex_data;
		(void)weight;
		//    std::cerr << "called combine" << std::endl;
		//VertexInfo* vertex = new VertexInfo(coords);
		//*out_data = vertex;
		//filled->extraVertices().push_back(vertex);
	}

	void Filled::errorCallback(GLenum error_code)
	{
		//std::cerr << "hmm. error during tessellation?:" << gluErrorString(error_code) << std::endl;
	}
#endif



//==============================================================================
// FontOutlineTessellator
//==============================================================================

//------------------------------------------------------------------------------
FontOutlineTessellator::FontOutlineTessellator()
{
	m_gluTesselator = gluNewTess();

	// �f�t�H���g�� GLU_TESS_WINDING_ODD.
	// �����A���ꂾ�Ɠʖʂ��d�Ȃ�悤�ȃO���t�őz��O�̌���������������B(���C���I��"��" ���킩��₷��)
	// �Ƃ肠���� GLU_TESS_WINDING_NONZERO �őS�����߂�悤�ɂ���B
	gluTessProperty(m_gluTesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

	gluTessCallback(m_gluTesselator, GLU_TESS_BEGIN_DATA, (GLUTessCallback)BeginCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_END_DATA, (GLUTessCallback)EndCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_VERTEX_DATA, (GLUTessCallback)VertexDataCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_COMBINE_DATA, (GLUTessCallback)CombineCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_ERROR, (GLUTessCallback)ErrorCallback);
}

//------------------------------------------------------------------------------
FontOutlineTessellator::~FontOutlineTessellator()
{
	gluDeleteTess(m_gluTesselator);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::Tessellate(RawFont::VectorGlyphInfo* info)
{
	TessellatingState state;
	state.thisPtr = this;
	state.glyphInfo = info;

	::gluTessBeginPolygon(m_gluTesselator, &state);

	for (auto& outline : info->outlines)
	{
		::gluTessBeginContour(m_gluTesselator);

		for (int i = 0; i < outline.vertexCount; i++)
		{
			int vertexIndex = outline.startIndex + i;
			const Vector2& v = info->vertices[vertexIndex].pos;
			GLfloat coords[3] = { v.x, v.y, 0 };
			::gluTessVertex(m_gluTesselator, coords, reinterpret_cast<void*>(vertexIndex));
		}

		::gluTessEndContour(m_gluTesselator);
	}

	::gluTessEndPolygon(m_gluTesselator);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::BeginCallback(GLenum primitiveType, TessellatingState* state)
{
	Contour c;
	c.primitiveType = primitiveType;
	c.intermediateVertexIndex1 = -1;
	c.intermediateVertexIndex2 = -1;
	c.faceCount = 0;
	state->contourList.Add(c);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::EndCallback(TessellatingState* state)
{

}

//------------------------------------------------------------------------------
// vertex : gluTessVertex() �� data
// �_�͎��v���ő����Ă���悤���B
void FontOutlineTessellator::VertexDataCallback(void* vertexData, TessellatingState* state)
{
	Contour* contour = &state->contourList.GetLast();
	int vertexIndex = reinterpret_cast<int>(vertexData);

	switch (contour->primitiveType)
	{
		// �ʏ�̎O�p�`���X�g�B3���_�����ɑ����Ă���B
		case GL_TRIANGLES:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				// 1�߂̓_
				contour->intermediateVertexIndex1 = vertexIndex;
				contour->intermediateVertexIndex2 = -1;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				// 2�߂̓_
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				// 3�̓_���ł���������
				state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex1);
				state->glyphInfo->triangleIndices.Add(vertexIndex);
				state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex1 = -1;
			}
			break;
		}
		// TriangleStrip �͍Ō��2�_���ė��p����B
		case GL_TRIANGLE_STRIP:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				contour->intermediateVertexIndex1 = vertexIndex;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				if (contour->faceCount & 1)	// ���
				{
					state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex1);
					state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex2);
					state->glyphInfo->triangleIndices.Add(vertexIndex);
				}
				else	// ������ (����܂�)
				{
					state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex1);
					state->glyphInfo->triangleIndices.Add(vertexIndex);
					state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex2);
				}

				contour->intermediateVertexIndex1 = contour->intermediateVertexIndex2;
				contour->intermediateVertexIndex2 = vertexIndex;
				contour->faceCount++;
			}
			break;
		}
		// TriangleFan �́A�ŏ��̓_�������ƍė��p����B
		case GL_TRIANGLE_FAN:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				contour->intermediateVertexIndex1 = vertexIndex;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex1);
				state->glyphInfo->triangleIndices.Add(vertexIndex);
				state->glyphInfo->triangleIndices.Add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::CombineCallback(GLfloat coords[3], void* vertex_data[4], GLfloat weight[4], void** out_data, TessellatingState* state)
{
	RawFont::FontOutlineVertex v(Vector2(coords[0], coords[1]));
	state->glyphInfo->vertices.Add(v);
	*out_data = reinterpret_cast<void*>(state->glyphInfo->vertices.GetCount() - 1);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::ErrorCallback(GLenum error_code)
{
	LN_ASSERT(0);	// TODO:
}


//==============================================================================
// FontOutlineStroker
//==============================================================================
//------------------------------------------------------------------------------
void FontOutlineStroker::MakeStroke(RawFont::VectorGlyphInfo* info)
{
	m_info = info;
	CalculateExtrusion();
	MakeAntiAliasStroke();
}

//------------------------------------------------------------------------------
void FontOutlineStroker::CalculateExtrusion()
{
	for (const auto& outline : m_info->outlines)
	{
		int end = outline.startIndex + outline.vertexCount;
		for (int i = outline.startIndex; i < end; i++)
		{
			int iPrev = i - 1;
			if (iPrev < outline.startIndex) iPrev = end - 1;
			int iNext = i + 1;
			if (iNext >= end) iNext = outline.startIndex;

			auto& cur = m_info->vertices[i];
			auto& prev = m_info->vertices[iPrev];
			auto& next = m_info->vertices[iNext];

			cur.pos += 0.5;

			Vector2 d0 = Vector2::Normalize(cur.pos - prev.pos);//cur.pos - prev.pos;//
			Vector2 d1 = Vector2::Normalize(next.pos - cur.pos);//next.pos - cur.pos;//
			//Vector2 v = prev.pos.x * next.pos.y - next.pos.x * prev.pos.y;
			//Vector2::clo
			float dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
			dlx0 = d0.y;//p0->dy;
			dly0 = -d0.x;
			dlx1 = d1.y;
			dly1 = -d1.x;
			// Calculate extrusions
			// �i�s�����̍���������
			cur.extrusion.x = -(dlx0 + dlx1) * 0.5f;
			cur.extrusion.y = 0;//-(dly0 + dly1) * 0.5f;
			cur.extrusion.Normalize();

			//cur.extrusion.x *= 0.75;
		}
	}
}

//------------------------------------------------------------------------------
void FontOutlineStroker::MakeAntiAliasStroke()
{
	for (const auto& outline : m_info->outlines)
	{
		int end = outline.startIndex + outline.vertexCount;

		float extRate = 0.8f;//0.5f;//1.0;//0.075;

		int i = outline.startIndex;
		auto& cur = m_info->vertices[i];

		int iNextExt = 0;
		int iStartExt = m_info->vertices.GetCount();
		int iCurExt = m_info->vertices.GetCount();




		//if (Math::NearEqual(cur.extrusion.x, 0.0f) || Math::NearEqual(cur.extrusion.y, 0.0f))
		//{
		//	m_info->vertices.Add(cur.pos);
		//	m_info->vertices.GetLast().alpha = 0.0f;
		//}
		//else
		{
			m_info->vertices.Add(cur.pos + cur.extrusion * extRate);
			m_info->vertices.GetLast().alpha = 0.0f;
		}


		m_info->vertices[i].pos -= m_info->vertices[i].extrusion * extRate * 0.5;
		//{
		//	m_info->vertices[i].pos -= m_info->vertices[i].extrusion * extRate;
		//}

		for (int i = outline.startIndex; i < end; i++)
		{
			//int iPrev = i - 1;
			//if (iPrev < outline.startIndex) iPrev = end - 1;
			int iNext = i + 1;
			if (iNext >= end) iNext = outline.startIndex;



			auto& cur = m_info->vertices[i];

			auto& next = m_info->vertices[iNext];


			//float extRate = 0.8f * cur.extrusion.y;

			if (iNext != outline.startIndex)	// start �͉����o���ς�
			{
				// next �̂������o��
				iNextExt = m_info->vertices.GetCount();

				//if (Math::NearEqual(next.pos.x - cur.pos.x, 0.0f) || Math::NearEqual(next.pos.y - cur.pos.y, 0.0f))
				//{
				//	m_info->vertices.Add(next.pos);
				//	m_info->vertices.GetLast().alpha = 0.0f;
				//}
				//else
				{
					m_info->vertices.Add(next.pos + next.extrusion * extRate);
					m_info->vertices.GetLast().alpha = 0.0f;
				}

				//m_info->vertices.Add(next.pos + next.extrusion * extRate);
				//m_info->vertices.GetLast().alpha = 0.0f;


				m_info->vertices[iNext].pos -= m_info->vertices[iNext].extrusion * extRate * 0.5;

			}
			else
			{
				iNextExt = iStartExt;
			}



			int i0 = i;
			int i1 = iNext;
			int i2 = iCurExt;
			int i3 = iNextExt;

			m_info->triangleIndices.Add(i0);
			m_info->triangleIndices.Add(i1);
			m_info->triangleIndices.Add(i2);

			m_info->triangleIndices.Add(i2);
			m_info->triangleIndices.Add(i1);
			m_info->triangleIndices.Add(i3);
			//Vector2 p0 = cur.pos;
			//Vector2 p1 = next.pos;
			//Vector2 p2 = m_vertexList[iCurExt].pos;

			iCurExt = iNextExt;
		}


		//return;
	}
}

} // namespace detail
LN_NAMESPACE_END
