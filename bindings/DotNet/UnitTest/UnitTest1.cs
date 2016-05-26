﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Lumino;

namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {
        [ClassInitialize()]
        public static void ClassInitialize(TestContext context)
        {
            Engine.Initialize();
        }

        [ClassCleanup()]
        public static void ClassCleanup()
        {
            Engine.Terminate();
        }

        /// <summary>
        /// 
        /// </summary>
        [TestMethod]
        public void Test_GetSetObject()
        {
            var tex1 = new Texture2D(32, 32);
            var spr1 = new Sprite2D();

            // set したもとの get で取得したものは同一
            spr1.Texture = tex1;
            Assert.AreEqual(tex1, spr1.Texture);

            //Assert.AreEqual(tex1, sprite.Texture);
            //Assert.AreEqual(tex1, sprite.Texture);	// 2回目以降も同じものが返ること
            //Assert.AreEqual(2, TestInterface.GetObjectWeakReferenceCount());

            //tex1 = null;
            //sprite = null;

            // TODO: 即GC実行する方法がイマイチわからないのでちょっと保留
            //GC.Collect(1, GCCollectionMode.Forced, true);
            //Assert.AreEqual(0, TestInterface.GetObjectWeakReferenceCount());
        }
    }
}
