﻿#pragma once
#include "UIElement.hpp"
#include "UILayoutElement.hpp"

namespace ln {
class UIItemsControl;

// TODO: GridLayout https://blog.qt.io/jp/2011/01/06/qml-layout/



/** グリッドレイアウトのセルサイズを指定する値の種類です。*/
enum class UILayoutLengthType
{
    Auto,				/**< 子要素のサイズに合わせる */
    Direct,				/**< サイズを直接指定する */
    Ratio,				/**< レイアウト後、残りの領域を使う */
};

namespace detail {
struct GridDefinitionData
{
    // input data
    UILayoutLengthType	type = UILayoutLengthType::Ratio;
    float			size = 0.0f;
    float			minSize = 0.0f;
    float			maxSize = FLT_MAX;

    // working data
    float			desiredSize = 0.0f;
    float			actualOffset = 0.0f;	// 最終オフセット
    float			actualSize = 0.0f;		// 最終サイズ

    float getAvailableDesiredSize() const
    {
        if (type == UILayoutLengthType::Auto) {
            return desiredSize;
        }
        else if (type == UILayoutLengthType::Direct) {
            return Math::clamp(size, minSize, maxSize);
        }
        else {
            return 0;
        }
    }

    float getRatioSize() const
    {
        return (size == 0.0f) ? 1.0f : size;
    }
};

} // namespace detail

class IUIElementList
{
public:
    virtual int getElementCount() const = 0;
    virtual UIElement* getElement(int i) const = 0;
};


/**
    @brief        ScrollViewer コントロール内のメインのスクロール可能領域を表します。
*/
// offset はスクロールバー基準。縦スクロールバーなら、Thumb が下に行くほど、VerticalOffset には + 方向の大きい値がセットされる。
class IScrollInfo
{
public:

    ///// 水平軸上にスクロールできるかどうかを示す値を設定します。
    ///// @note    スクロールバーを表示しない設定になっていたり、そもそも存在しない場合に false がセットされる。
    /////            IScrollInfo の実装クラスで不要な計算を行わないなどのために参照する。
    //virtual void SetCanHorizontallyScroll(bool enabled) = 0;

    ///// 水平軸上にスクロールできるかどうかを示す値を取得します。
    //virtual bool CanHorizontallyScroll() const = 0;

    ///// 垂直軸上にスクロールできるかどうかを示す値を設定します。
    //virtual void SetCanVerticallyScroll(bool enabled) = 0;

    ///// 垂直軸上にスクロールできるかどうかを示す値を取得します。
    //virtual bool CanVerticallyScroll() const = 0;

    /// エクステント (コンテンツ全体) の横幅を取得します。
    virtual float getExtentWidth() const = 0;

    /// エクステント (コンテンツ全体) の縦幅を取得します。
    virtual float getExtentHeight() const = 0;

    /// コンテンツに対する実際の表示領域の横幅を取得します。
    virtual float getViewportWidth() const = 0;

    /// コンテンツに対する実際の表示領域の縦幅を取得します。
    virtual float getViewportHeight() const = 0;

    /// スクロールしたコンテンツの水平オフセットを設定します。
    virtual void setHorizontalOffset(float offset) = 0;

    /// スクロールしたコンテンツの水平オフセットを取得します。
    virtual float getHorizontalOffset() const = 0;

    /// スクロールしたコンテンツの水平オフセットを設定します。
    virtual void setVerticalOffset(float offset) = 0;

    /// スクロールしたコンテンツの垂直オフセットを取得します。
    virtual float getVerticalOffset() const = 0;

    /// スクロール動作を制御する ScrollViewer 要素を設定します。
    //virtual void SetScrollOwner(ScrollViewer* owner) = 0;

#if 0
/// コンテンツ内を 1 論理単位ずつ上にスクロールします。
    virtual void LineUp() = 0;

    /// コンテンツ内を 1 論理単位ずつ下にスクロールします。
    virtual void LineDown() = 0;

    /// コンテンツ内を 1 論理単位ずつ左にスクロールします。
    virtual void LineLeft() = 0;

    /// コンテンツ内を 1 論理単位ずつ右にスクロールします。
    virtual void LineRight() = 0;

    /// コンテンツ内を 1 ページずつ上にスクロールします。
    virtual void PageUp() = 0;

    /// コンテンツ内を 1 ページずつ下にスクロールします。
    virtual void PageDown() = 0;

    /// コンテンツ内を 1 ページずつ左にスクロールします。
    virtual void PageLeft() = 0;

    /// コンテンツ内を 1 ページずつ右にスクロールします。
    virtual void PageRight() = 0;

    /// コンテンツ内をマウスホイール操作 1 回分、上にスクロールします。
    virtual void MouseWheelUp() = 0;

    /// コンテンツ内をマウスホイール操作 1 回分、下にスクロールします。
    virtual void MouseWheelDown() = 0;

    /// コンテンツ内をマウスホイール操作 1 回分、左にスクロールします。
    virtual void MouseWheelLeft() = 0;

    /// コンテンツ内をマウスホイール操作 1 回分、右にスクロールします。
    virtual void MouseWheelRight() = 0;
#endif

    //Rect MakeVisible(UIElement visual, Rect rectangle);
};



// TODO: ネスト
class UILayoutPanel2
    : public UIElement
    , public IScrollInfo
{
public:
    UILayoutPanel2();
    void init();

    virtual void addChild(UIElement* child) override;
    void removeChild(UIElement* child);
    void removeAllChildren();

protected:
    virtual void onDispose(bool explicitDisposing) override;

    // IScrollInfo interface
    virtual float getExtentWidth() const;
    virtual float getExtentHeight() const;
    virtual float getViewportWidth() const;
    virtual float getViewportHeight() const;
    virtual void setHorizontalOffset(float offset);
    virtual float getHorizontalOffset() const;
    virtual void setVerticalOffset(float offset);
    virtual float getVerticalOffset() const;

public:	// TODO:
    List<Ref<UIElement>> m_logicalChildren;
    Vector2 m_scrollOffset;
    UIItemsControl* m_ownerItemsControl;

    friend class UIItemsControl;
};

class UIFrameLayout2	// TODO: BorderLayout の方がいいかも https://doc.qt.io/qt-5/qtwidgets-layouts-borderlayout-example.html
    : public UILayoutPanel2
{
public:
    static Ref<UIFrameLayout2> create();

    // TODO: internal
    static Size staticMeasureOverride(UIElement* ownerElement, const Size& constraint);
    static Size staticArrangeOverride(UIElement* ownerElement, const Size& finalSize);

LN_PROTECTED_INTERNAL_ACCESS:
    UIFrameLayout2();
    void init();

    // UIElement interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

private:
};

// TODO: 一方向の GridLayout は BoxLayout (UE4) にする。これは、指定方向のレイアウト領域を inf にしない。
// 一方、WPF の StackPanel のように、細かい配置の調整はせずただ並べるだけのものは LinearLayout(Andorid) にしてみる。これは指定方向のレイアウト領域が inf になる。
// おもに ListView など用。
class UIStackLayout2
    : public UILayoutPanel2
{
public:
    static Ref<UIStackLayout2> create();

    void setOrientation(Orientation orientation) { m_orientation = orientation; }
    Orientation getOrientation() const { return m_orientation; }

    void addChild(UIElement* element) { addChild(element, UILayoutLengthType::Auto); }
    void addChild(UIElement* element, UILayoutLengthType type);

LN_CONSTRUCT_ACCESS:
    UIStackLayout2();
    void init();

    // UIElement interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

private:
    bool isHorizontal() const { return m_orientation == Orientation::Horizontal || m_orientation == Orientation::ReverseHorizontal; }

    struct CellDefinition
    {
        // input data (initial data)
        UILayoutLengthType type = UILayoutLengthType::Auto;
        float size = 1.0f;
        float minSize = 0.0f;
        float maxSize = FLT_MAX;

        // working data
        float desiredSize = Math::NaN;
        float actualOffset = Math::NaN;
        float actualSize = Math::NaN;
    };

    Orientation m_orientation;
    List<CellDefinition> m_cellDefinitions;
};

class UIVBoxLayout2
    : public UIStackLayout2
{
public:

LN_CONSTRUCT_ACCESS:
    void init();
};

class UIHBoxLayout2
	: public UIStackLayout2
{
public:

LN_CONSTRUCT_ACCESS:
	void init();
};



// 指定方向に無限のサイズを想定する StackLayout と異なり、こちらは有限サイズでレイアウトを行う。
// 特に末尾の要素を下揃えしたり、指定方向への Stratch をできるようにしたりする。
// このため ListBox などの HostPanel には向かない。
class UIBoxLayout3
    : public UILayoutPanel2
{
public:
    void setOrientation(Orientation orientation) { m_orientation = orientation; }
    Orientation getOrientation() const { return m_orientation; }

LN_CONSTRUCT_ACCESS:
    UIBoxLayout3();
    void init();

    // UIElement interface
    virtual Size measureOverride(const Size& constraint) override;
    virtual Size arrangeOverride(const Size& finalSize) override;

private:
    bool isHorizontal() const { return m_orientation == Orientation::Horizontal || m_orientation == Orientation::ReverseHorizontal; }
    UILayoutLengthType layoutType(int index) const;
    float layoutWeight(int index) const;
    float layoutDirectSize(int index) const;
    void getLayoutMinMaxSize(int index, float* minSize, float* maxSize) const;

    struct CellDefinition
    {
        float desiredSize = Math::NaN;
        float actualOffset = Math::NaN;
        float actualSize = Math::NaN;
    };

    Orientation m_orientation;
    List<CellDefinition> m_cellDefinitions;
};










class UISwitchLayout
	: public UIFrameLayout2
{
public:
	void setActiveIndex(int index);
    void setActive(UIElement* child);

protected:
	// base interface
	virtual Size measureOverride(const Size& constraint) override;

LN_CONSTRUCT_ACCESS:
	UISwitchLayout();
    void init();

private:
	int m_activeIndex;
};







// TODO: ネスト
class UILayoutPanel
    : public Object
    , public IScrollInfo
{
public:
    UILayoutPanel();
    void init();

    /** レイアウト処理の測定パスの実行中にこの要素が計算したサイズを取得します。この値は子要素が親要素へ要求する、子要素自身の最低サイズです。*/
    const Size& desiredSize() const { return m_desiredSize; }


    // TODO: internal
    void measureLayout(const IUIElementList* childElements, const Size& availableSize);
    void arrangeLayout(const IUIElementList* childElements, const Rect& finalSlotRect);

    virtual Size measureOverride(const IUIElementList* childElements, const Size& constraint) = 0;
    virtual Size arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect) = 0;

protected:
    // IScrollInfo interface
    virtual float getExtentWidth() const;
    virtual float getExtentHeight() const;
    virtual float getViewportWidth() const;
    virtual float getViewportHeight() const;
    virtual void setHorizontalOffset(float offset);
    virtual float getHorizontalOffset() const;
    virtual void setVerticalOffset(float offset);
    virtual float getVerticalOffset() const;

public:	// TODO: internal

private:
    Size m_desiredSize;
    Size m_actualSize;
    Vector2 m_scrollOffset;
};

class UIFrameLayout	// TODO: BorderLayout の方がいいかも https://doc.qt.io/qt-5/qtwidgets-layouts-borderlayout-example.html
    : public UILayoutPanel
{
public:
    static Ref<UIFrameLayout> create();

LN_CONSTRUCT_ACCESS:
    UIFrameLayout();
    virtual ~UIFrameLayout();
    void init();

    virtual Size measureOverride(const IUIElementList* childElements, const Size& constraint) override;
    virtual Size arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect) override;

private:
};

// Orientation=H で大量アイテムを持つ ListView を配置すると、縦方向スクロールバーが表示される。
// Orientation=V で大量アイテムを持つ ListView を配置すると、スクロールバーは表示されず、すべてのアイテムを包含するサイズが ListView の actualSize となる。（見切れる）
class UIStackLayout
    : public UILayoutPanel
{
public:
    static Ref<UIStackLayout> create();

    void setOrientation(Orientation orientation) { m_orientation = orientation; }
    Orientation getOrientation() const { return m_orientation; }

    bool lastStretch = false;

LN_CONSTRUCT_ACCESS:
    UIStackLayout();
    virtual ~UIStackLayout();
    void init();

    virtual Size measureOverride(const IUIElementList* childElements, const Size& constraint) override;
    virtual Size arrangeOverride(const IUIElementList* childElements, const Rect& finalSlotRect) override;

private:
    Orientation m_orientation;
};

class UIVBoxLayout
    : public UIStackLayout
{
public:

LN_CONSTRUCT_ACCESS:
    void init();
};

} // namespace ln

