﻿/*
  [2020/4/7]
  結局 WPF のようなプロパティ単位で極限まで抽象化されたアニメーションの実装はやめることにした。
  メンテナンスの難易度もそうだが、文字列検索が頻繁に発生したり、パフォーマンスへの影響も大きい。

  なので、Flutter や Android を参考にしつつ、CSS の形に寄せてみることにした。
  簡単に言うと定義済みのプロパティに対してのみアニメーションを適用できるようにする。

  例えば次のような具合。
  setBackgroundColorAnimation(Color::Red, 1.0);
  setOpacity(0.5, 1.0);
  setFontSize(20, 0.5);
  setSize(100, 2.0);    // setWidth() setHeight() の統合ユーティリティ

  基本単位。
  UIStyleAnimationKey
    parcent,
    Optional<backgroundColor>,
    Optional<opacity>,
    Optional<fontSize>,
    Optional<width>,
    Optional<height>,
  細かくするとパフォーマンスが気になるなら、Flutter のように Width, Height をまとめて Size にするとかしてもよい。

  UIStyleAnimationTrack
    CSS の @keyframes 相当。UIStyleAnimationKey のリスト。

  UIAnimationClip
    animation: に , 区切りで指定する 1 要素。
    ひとつの UIStyleAnimationTrack と、開始時間や t の生成方法などを指定する。


    Animation には２種類ある。
    - animation: キーフレームアニメーション。値はすべて事前定義。
    - transition: アクティブになったら、現在値から目標値へ遷移する。



    Animation は Style の要素単位で持つ。
    "Vector3 position" ではなく、"float positionX, positionY, positionZ" のようにプリミティブ単位で。
    こうしておかないと、たとえば x y 「どちらかだけ省略する」ができなくなる。
    Size を例にすると、高さは Alignment.Center で決めたいが幅はマウスオーバーしたらアニメ付きで伸ばしたい、といったときに弊害になる。


    適用時は、
    StyleInstance に TransitionInstance を、こちらも要素単位で持たせておく。
    AnimationInstance は、combinedStyle に集まってくる AnimationElement (ポインタ)をキーに作る。revsiton 見てもいいかも。

    Animation と Transition の同時指定は無効。
    Style に一応設定はできるが、Instance を作るときは Transition が優先される。



    Color は要素ごとにアニメしなくていいかもしれない。というか要素単位アニメほとんど使わない気がする。CSSでも無いし。
*/
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIStyleAnimation.hpp>

namespace ln {

//==============================================================================
// UITransitionClip

UIAnimationClip::UIAnimationClip()
{
}

bool UIAnimationClip::init()
{
    return Object::init();
}

void UIAnimationClip::addCurve(detail::UIStyleAnimationElement element, float startValue, float targetValue, float duration, EasingMode func, AnimationWrapMode wrapMode)
{
    m_curves[element] = EasingAnimationCurve::create(startValue, targetValue, duration, func);
    m_curves[element]->setWrapMode(wrapMode);
}

//==============================================================================
// UITransitionClip

UITransitionClip::UITransitionClip()
{
}

bool UITransitionClip::init()
{
    return Object::init();
}

void UITransitionClip::addCurve(detail::UIStyleAnimationElement element, float targetValue, float duration, EasingMode func, float delay)
{
    m_curves[element] = Curve{ EasingFunctions::function(func), targetValue, duration, delay };
}

} // namespace ln

