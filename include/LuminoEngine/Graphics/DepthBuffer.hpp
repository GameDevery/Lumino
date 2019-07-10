﻿// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class IDepthBuffer;
}

/** 深度バッファのクラスです。 */
class DepthBuffer
    : public GraphicsResource
{
public:
    /**
     * 深度バッファを作成します。
     * @param[in]   width   : 幅 (px 単位)
     * @param[in]   height  : 高さ (px 単位)
     */
    static Ref<DepthBuffer> create(int width, int height);

    /** 一時的な DepthBuffer を取得します。 */
    static Ref<DepthBuffer> getTemporary(int width, int height);

    /** getTemporary で取得した一時的な DepthBuffer を解放します。 */
    static void releaseTemporary(DepthBuffer* depthBuffer);

    /** 幅を取得します。(ピクセル単位) */
    int width() const { return m_size.width; }

    /** 高さを取得します。 (ピクセル単位) */
    int height() const { return m_size.height; }

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	DepthBuffer();
    virtual ~DepthBuffer();

    /** @copydoc create(int, int) */
    void init(int width, int height);

private:
    detail::IDepthBuffer* resolveRHIObject(GraphicsContext* context, bool* outModified);

    Ref<detail::IDepthBuffer> m_rhiObject;
    SizeI m_size;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln