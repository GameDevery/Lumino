# PixelFormat
module Lumino::PixelFormat
    # Unknown
    UNKNOWN = 0
    # 8bit アルファ値のみのフォーマット
    A8 = 1
    # RGBA オーダーの各要素 8bit フォーマット
    RGBA8 = 2
    # RGB オーダーの各要素 8bit フォーマット
    RGB8 = 3
    # RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    RGBA32F = 4
end

# TextureFormat
module Lumino::TextureFormat
    # Unknown
    UNKNOWN = 0
    # RGBA オーダーの各要素 8bit フォーマット
    RGBA8 = 1
    # RGB オーダーの各要素 8bit フォーマット
    RGB8 = 2
    # RGBA オーダーの各要素 16bit 浮動小数点フォーマット
    RGBA16F = 3
    # RGBA オーダーの各要素 32bit 浮動小数点フォーマット
    RGBA32F = 4
    # 16bit 浮動小数点フォーマット
    R16F = 5
    # 32bit 浮動小数点フォーマット
    R32F = 6
    # 32bit の符号なし整数フォーマット
    R32U = 7
end

# DepthBufferFormat
module Lumino::DepthBufferFormat
    # S8 32 ビット
    D24S8 = 0
end

# アプリケーション全体にかかわる処理を行います。
# 
class Lumino::Engine
    # エンジンの初期化処理を行います。
    def initialize()
    end

    # エンジンの終了処理を行います。
    def finalize()
    end

    # 
    # @return [Boolean] アプリケーションの終了が要求されている場合は false を返します。
    def update()
    end

    # 。
    # @return [Lumino::UIControl] 
    def main_ui_view()
    end

end

# Graphics 機能に関係するリソースのベースクラスです。
# 
class Lumino::GraphicsResource
end

# テクスチャのベースクラスです。
# 
class Lumino::Texture < GraphicsResource
end

# 2D テクスチャのクラスです。
# 
class Lumino::Texture2D < Texture
    # テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    # @param [Integer] width 
    # @param [Integer] height 
    def initialize(width, height)
    end
    # テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    # @param [Integer] width 
    # @param [Integer] height 
    # @param [Lumino::TextureFormat] format 
    def initialize(width, height, format)
    end
    # テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
    # @param [String] filePath 
    # @param [Lumino::TextureFormat] format 
    def initialize(filePath, format)
    end

end

# 
# 
class Lumino::Component
end

# 
# 
class Lumino::VisualComponent < Component
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @param [Boolean] value 
    def visible=(value)
    end

    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?()
    end

end

# 
# 
class Lumino::SpriteComponent < VisualComponent
    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] texture 
    def texture=(texture)
    end

end

# 
# 
class Lumino::ComponentList
    # 
    # @return [Integer] 
    def get_length()
    end

    # 
    # @param [Integer] index 
    # @return [Lumino::Component] 
    def get_item(index)
    end

end

# World 内の全エンティティのベースクラスです。
# 
class Lumino::WorldObject
    # このオブジェクトの位置を設定します。
    # @param [Lumino::Vector3] pos 
    def position=(pos)
    end
    # このオブジェクトの位置を設定します。
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def position=(x, y, z)
    end

    # このオブジェクトの位置を位置を取得します。
    # @return [Lumino::Vector3] 
    def position()
    end

    # このオブジェクトの回転を設定します。
    # @param [Lumino::Quaternion] rot 
    def rotation=(rot)
    end

    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def set_euler_angles(x, y, z)
    end

    # このオブジェクトの回転を取得します。
    # @return [Lumino::Quaternion] 
    def rotation()
    end

    # このオブジェクトの拡大率を設定します。
    # @param [Lumino::Vector3] scale 
    def scale=(scale)
    end
    # このオブジェクトの拡大率を設定します。
    # @param [Float] xyz 
    def scale=(xyz)
    end
    # このオブジェクトの拡大率を設定します。
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def scale=(x, y, z)
    end

    # このオブジェクトの拡大率を取得します。
    # @return [Lumino::Vector3] 
    def scale()
    end

    # このオブジェクトのローカルの中心位置を設定します。
    # @param [Lumino::Vector3] value 
    def center_point=(value)
    end
    # このオブジェクトのローカルの中心位置を設定します。
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def center_point=(x, y, z)
    end

    # このオブジェクトのローカルの中心位置を取得します。
    # @return [Lumino::Vector3] 
    def center_point()
    end

    # 
    # @return [Lumino::ComponentList] 
    def components()
    end

    # フレーム更新
    # @param [Float] elapsedSeconds 
    def on_update(elapsedSeconds)
    end

end

# VisualComponent がアタッチされた WorldObject のプレハブクラスです。
# 
class Lumino::VisualObject < WorldObject
    # 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
    # @param [Boolean] value 
    def visible=(value)
    end

    # 可視状態を取得します。
    # @return [Boolean] 
    def visible?()
    end

end

# スプライトオブジェクトを表します。
# 
class Lumino::Sprite < VisualObject
    # スプライトが表示するテクスチャを設定します。
    # @param [Lumino::Texture] texture 
    def texture=(texture)
    end

    # 
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] width 
    # @param [Float] height 
    def set_source_rect(x, y, width, height)
    end

    # init
    # @param [Lumino::Texture] texture 
    # @param [Float] width 
    # @param [Float] height 
    def initialize(texture, width, height)
    end

end

# 
# 
class Lumino::UIEventArgs
    # イベントの発生元となった要素を取得します。
    # @return [Lumino::UIElement] 
    def sender()
    end

end

# 
# 
class Lumino::UILayoutElement
end

# 
# 
class Lumino::UIElement < UILayoutElement
    # このオブジェクトの位置を設定します。
    # @param [Lumino::Vector3] pos 
    def position=(pos)
    end
    # このオブジェクトの位置を設定します。
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def position=(x, y, z)
    end

    # このオブジェクトの位置を位置を取得します。
    # @return [Lumino::Vector3] 
    def position()
    end

    # このオブジェクトの回転を設定します。
    # @param [Lumino::Quaternion] rot 
    def rotation=(rot)
    end

    # このオブジェクトの回転をオイラー角から設定します。(radian)
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def set_euler_angles(x, y, z)
    end

    # このオブジェクトの回転を取得します。
    # @return [Lumino::Quaternion] 
    def rotation()
    end

    # このオブジェクトの拡大率を設定します。
    # @param [Lumino::Vector3] scale 
    def scale=(scale)
    end
    # このオブジェクトの拡大率を設定します。
    # @param [Float] xyz 
    def scale=(xyz)
    end
    # このオブジェクトの拡大率を設定します。
    # @param [Float] x 
    # @param [Float] y 
    def scale=(x, y)
    end

    # このオブジェクトの拡大率を取得します。
    # @return [Lumino::Vector3] 
    def scale()
    end

    # このオブジェクトのローカルの中心位置を設定します。
    # @param [Lumino::Vector3] value 
    def center_point=(value)
    end
    # このオブジェクトのローカルの中心位置を設定します。
    # @param [Float] x 
    # @param [Float] y 
    # @param [Float] z 
    def center_point=(x, y, z)
    end

    # このオブジェクトのローカルの中心位置を取得します。
    # @return [Lumino::Vector3] 
    def center_point()
    end

    # Add element to container.
    # @param [Lumino::UIElement] child 
    def add_child(child)
    end

end

# 視覚情報の表示やユーザー入力の処理を行う要素の基本クラスです。
# 
class Lumino::UIControl < UIElement
end

# 
# 
class Lumino::UIButtonBase < UIControl
    # set text.
    # @param [String] text 
    def set_text(text)
    end

end

# 
# 
class Lumino::UIButton < UIButtonBase
    # init.
    def initialize()
    end

    # Clicked イベントの通知を受け取るコールバックを登録します。
    # @param [Lumino::UIEventHandler] handler 
    # @return [] 
    def connect_on_clicked(handler)
    end

end

