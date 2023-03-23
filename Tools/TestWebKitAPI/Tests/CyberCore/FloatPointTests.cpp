/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include <CyberCore/AffineTransform.h>
#include <CyberCore/FloatPoint.h>
#include <CyberCore/FloatSize.h>
#include <CyberCore/IntPoint.h>
#include <CyberCore/IntSize.h>
#include <CyberCore/TransformationMatrix.h>

#if USE(CG)
#include <CoreGraphics/CoreGraphics.h>
#endif

namespace TestCyberKitAPI {

static void testGetAndSet(CyberCore::FloatPoint point)
{
    point.setX(1.1f);
    EXPECT_FLOAT_EQ(1.1f, point.x());
    point.setY(2.2f);
    EXPECT_FLOAT_EQ(2.2f, point.y());

    point.set(9.9f, 8.8f);
    EXPECT_FLOAT_EQ(9.9f, point.x());
    EXPECT_FLOAT_EQ(8.8f, point.y());
}

TEST(FloatPoint, DefaultConstruction)
{
    CyberCore::FloatPoint test;

    EXPECT_FLOAT_EQ(0, test.x());
    EXPECT_FLOAT_EQ(0, test.y());

    testGetAndSet(test);
}

TEST(FloatPoint, ValueConstruction)
{
    CyberCore::FloatPoint test(9.9f, 8.8f);

    EXPECT_FLOAT_EQ(9.9f, test.x());
    EXPECT_FLOAT_EQ(8.8f, test.y());

    testGetAndSet(test);
}

TEST(FloatPoint, ZeroConstruction)
{
    CyberCore::FloatPoint test = CyberCore::FloatPoint::zero();

    EXPECT_FLOAT_EQ(0, test.x());
    EXPECT_FLOAT_EQ(0, test.y());
}

TEST(FloatPoint, IntPointConstruction)
{
    CyberCore::IntPoint testInput(2003, 1997);
    CyberCore::FloatPoint test = CyberCore::FloatPoint(testInput);

    EXPECT_FLOAT_EQ(2003.0f, test.x());
    EXPECT_FLOAT_EQ(1997.0f, test.y());
}

TEST(FloatPoint, FloatSizeConstruction)
{
    CyberCore::FloatSize testInput(500.7f, 300.2f);
    CyberCore::FloatPoint test = CyberCore::FloatPoint(testInput);

    EXPECT_FLOAT_EQ(500.7, test.x());
    EXPECT_FLOAT_EQ(300.2, test.y());
}

TEST(FloatPoint, MoveByFloat)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    test.move(20.2f, 30.3f);

    EXPECT_FLOAT_EQ(120.2f, test.x());
    EXPECT_FLOAT_EQ(230.3f, test.y());

    test.move(-81.3f, 10.0f);

    EXPECT_FLOAT_EQ(38.9f, test.x());
    EXPECT_FLOAT_EQ(240.3f, test.y());

    test.move(11.1f, -33.2f);

    EXPECT_FLOAT_EQ(50.0f, test.x());
    EXPECT_FLOAT_EQ(207.1f, test.y());

    test.move(-5.6f, -9.8f);

    EXPECT_FLOAT_EQ(44.4f, test.x());
    EXPECT_FLOAT_EQ(197.3f, test.y());
}

TEST(FloatPoint, MoveByIntSize)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    CyberCore::IntSize first(20, 30);

    test.move(first);

    EXPECT_FLOAT_EQ(120.0f, test.x());
    EXPECT_FLOAT_EQ(230.0f, test.y());

    CyberCore::IntSize second(-81, 10);

    test.move(second);

    EXPECT_FLOAT_EQ(39.0f, test.x());
    EXPECT_FLOAT_EQ(240.0f, test.y());

    CyberCore::IntSize third(11, -33);

    test.move(third);

    EXPECT_FLOAT_EQ(50.0f, test.x());
    EXPECT_FLOAT_EQ(207.0f, test.y());

    CyberCore::IntSize fourth(-6, -10);

    test.move(fourth);

    EXPECT_FLOAT_EQ(44.0f, test.x());
    EXPECT_FLOAT_EQ(197.0f, test.y());
}

TEST(FloatPoint, MoveByFloatSize)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    CyberCore::FloatSize first(20.2f, 30.3f);

    test.move(first);

    EXPECT_FLOAT_EQ(120.2f, test.x());
    EXPECT_FLOAT_EQ(230.3f, test.y());

    CyberCore::FloatSize second(-81.3f, 10.0f);

    test.move(second);

    EXPECT_FLOAT_EQ(38.9f, test.x());
    EXPECT_FLOAT_EQ(240.3f, test.y());

    CyberCore::FloatSize third(11.1f, -33.2f);

    test.move(third);

    EXPECT_FLOAT_EQ(50.0f, test.x());
    EXPECT_FLOAT_EQ(207.1f, test.y());

    CyberCore::FloatSize fourth(-5.6f, -9.8f);

    test.move(fourth);

    EXPECT_FLOAT_EQ(44.4f, test.x());
    EXPECT_FLOAT_EQ(197.3f, test.y());
}

TEST(FloatPoint, MoveByIntPoint)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    CyberCore::IntPoint first(20, 30);

    test.moveBy(first);

    EXPECT_FLOAT_EQ(120.0f, test.x());
    EXPECT_FLOAT_EQ(230.0f, test.y());

    CyberCore::IntPoint second(-81, 10);

    test.moveBy(second);

    EXPECT_FLOAT_EQ(39.0f, test.x());
    EXPECT_FLOAT_EQ(240.0f, test.y());

    CyberCore::IntPoint third(11, -33);

    test.moveBy(third);

    EXPECT_FLOAT_EQ(50.0f, test.x());
    EXPECT_FLOAT_EQ(207.0f, test.y());

    CyberCore::IntPoint fourth(-6, -10);

    test.moveBy(fourth);

    EXPECT_FLOAT_EQ(44.0f, test.x());
    EXPECT_FLOAT_EQ(197.0f, test.y());
}

TEST(FloatPoint, MoveByFloatPoint)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    CyberCore::FloatPoint first(20.2f, 30.3f);

    test.moveBy(first);

    EXPECT_FLOAT_EQ(120.2f, test.x());
    EXPECT_FLOAT_EQ(230.3f, test.y());

    CyberCore::FloatPoint second(-81.3f, 10.0f);

    test.moveBy(second);

    EXPECT_FLOAT_EQ(38.9f, test.x());
    EXPECT_FLOAT_EQ(240.3f, test.y());

    CyberCore::FloatPoint third(11.1f, -33.2f);

    test.moveBy(third);

    EXPECT_FLOAT_EQ(50.0f, test.x());
    EXPECT_FLOAT_EQ(207.1f, test.y());

    CyberCore::FloatPoint fourth(-5.6f, -9.8f);

    test.moveBy(fourth);

    EXPECT_FLOAT_EQ(44.4f, test.x());
    EXPECT_FLOAT_EQ(197.3f, test.y());
}

TEST(FloatPoint, Scale)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    test.scale(2.0f, 2.0f);

    EXPECT_FLOAT_EQ(200.0f, test.x());
    EXPECT_FLOAT_EQ(400.0f, test.y());

    test.scale(0.5f, 0.5f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    test.scale(2);

    EXPECT_FLOAT_EQ(200.0f, test.x());
    EXPECT_FLOAT_EQ(400.0f, test.y());

    test.scale(1.0f, 0.5f);

    EXPECT_FLOAT_EQ(200.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());
}

TEST(FloatPoint, Normalize)
{
    CyberCore::FloatPoint a(30.0f, 40.0f);

    a.normalize();

    EXPECT_FLOAT_EQ(0.6, a.x());
    EXPECT_FLOAT_EQ(0.8, a.y());
}

TEST(FloatPoint, Dot)
{
    CyberCore::FloatPoint a(100.0f, 200.0f);
    CyberCore::FloatPoint b(2.0f, 4.0f);
    CyberCore::FloatPoint c(1.0f, 0.5f);

    EXPECT_NEAR(1000.0f, a.dot(b), 0.0001f);
    EXPECT_NEAR(200.0f, a.dot(c), 0.0001f);
}

TEST(FloatPoint, LengthSquared)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    EXPECT_FLOAT_EQ(50000.0f, test.lengthSquared());
}

TEST(FloatPoint, ConstrainedBetween)
{
    CyberCore::FloatPoint left(10.0f, 20.0f);
    CyberCore::FloatPoint right(100.0f, 200.0f);

    CyberCore::FloatPoint test1(50.0f, 80.0f);
    CyberCore::FloatPoint test2(8.0f, 80.0f);
    CyberCore::FloatPoint test3(50.0f, 220.0f);

    auto constrained1 = test1.constrainedBetween(left, right);
    EXPECT_FLOAT_EQ(50.0f, constrained1.x());
    EXPECT_FLOAT_EQ(80.0f, constrained1.y());

    auto constrained2 = test2.constrainedBetween(left, right);
    EXPECT_FLOAT_EQ(10.0f, constrained2.x());
    EXPECT_FLOAT_EQ(80.0f, constrained2.y());

    auto constrained3 = test3.constrainedBetween(left, right);
    EXPECT_FLOAT_EQ(50.0f, constrained3.x());
    EXPECT_FLOAT_EQ(200.0f, constrained3.y());
}

TEST(FloatPoint, ShrunkTo)
{
    CyberCore::FloatPoint big(100.0f, 200.0f);
    CyberCore::FloatPoint smaller(10.0f, 80.0f);

    auto shrunkTo = big.shrunkTo(smaller);

    EXPECT_FLOAT_EQ(10.0f, shrunkTo.x());
    EXPECT_FLOAT_EQ(80.0f, shrunkTo.y());

    CyberCore::FloatPoint bigish(8.0f, 200.0f);

    auto shrunkTo2 = bigish.shrunkTo(smaller);

    EXPECT_FLOAT_EQ(8.0f, shrunkTo2.x());
    EXPECT_FLOAT_EQ(80.0f, shrunkTo2.y());
}

TEST(FloatPoint, ExpandedTo)
{
    CyberCore::FloatPoint big(100.0f, 200.0f);
    CyberCore::FloatPoint smaller(10.0f, 80.0f);

    auto expandedTo = smaller.expandedTo(big);

    EXPECT_FLOAT_EQ(100.0f, expandedTo.x());
    EXPECT_FLOAT_EQ(200.0f, expandedTo.y());

    CyberCore::FloatPoint bigish(8.0f, 300.0f);

    auto expandedTo2 = bigish.expandedTo(big);

    EXPECT_FLOAT_EQ(100.0f, expandedTo2.x());
    EXPECT_FLOAT_EQ(300.0f, expandedTo2.y());
}

TEST(FloatPoint, Transpose)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    EXPECT_FLOAT_EQ(100.0f, test.x());
    EXPECT_FLOAT_EQ(200.0f, test.y());

    auto transposed = test.transposedPoint();

    EXPECT_FLOAT_EQ(200.0f, transposed.x());
    EXPECT_FLOAT_EQ(100.0f, transposed.y());
}

TEST(FloatPoint, Transforms)
{
    CyberCore::FloatPoint test(100.0f, 200.0f);

    CyberCore::AffineTransform affine;

    auto transformed1 = test.matrixTransform(affine);

    EXPECT_FLOAT_EQ(100.0f, transformed1.x());
    EXPECT_FLOAT_EQ(200.0f, transformed1.y());

    CyberCore::AffineTransform affine2(2.0, 0.0, 0.0, 2.0, 0.0, 0.0);

    auto transformed2 = test.matrixTransform(affine2);

    EXPECT_FLOAT_EQ(200.0f, transformed2.x());
    EXPECT_FLOAT_EQ(400.0f, transformed2.y());

    CyberCore::TransformationMatrix matrix;

    auto transformed3 = test.matrixTransform(matrix);

    EXPECT_FLOAT_EQ(100.0f, transformed3.x());
    EXPECT_FLOAT_EQ(200.0f, transformed3.y());

    auto transformed4 = test.matrixTransform(affine2.toTransformationMatrix());

    EXPECT_FLOAT_EQ(200.0f, transformed4.x());
    EXPECT_FLOAT_EQ(400.0f, transformed4.y());
}

TEST(FloatPoint, Math)
{
    CyberCore::FloatPoint a(100.0f, 200.0f);
    CyberCore::FloatPoint b(100.0f, 200.0f);

    a += b;

    EXPECT_FLOAT_EQ(200.0f, a.x());
    EXPECT_FLOAT_EQ(400.0f, a.y());

    CyberCore::FloatSize c(50.0f, 50.0f);

    a += c;

    EXPECT_FLOAT_EQ(250.0f, a.x());
    EXPECT_FLOAT_EQ(450.0f, a.y());

    CyberCore::FloatSize d(10.0f, 200.0f);

    a -= d;

    EXPECT_FLOAT_EQ(240.0f, a.x());
    EXPECT_FLOAT_EQ(250.0f, a.y());

    CyberCore::FloatSize e(100.0f, 200.0f);

    auto f = b + e;

    EXPECT_FLOAT_EQ(200.0f, f.x());
    EXPECT_FLOAT_EQ(400.0f, f.y());

    CyberCore::FloatPoint g(10.0f, 20.0f);

    auto h = b + g;

    EXPECT_FLOAT_EQ(110.0f, h.x());
    EXPECT_FLOAT_EQ(220.0f, h.y());

    CyberCore::FloatSize i = b - g;

    EXPECT_FLOAT_EQ(90.0f, i.width());
    EXPECT_FLOAT_EQ(180.0f, i.height());

    CyberCore::FloatPoint j = b - e;

    EXPECT_FLOAT_EQ(0.0f, j.x());
    EXPECT_FLOAT_EQ(0.0f, j.y());

    CyberCore::FloatPoint negated = -b;

    EXPECT_FLOAT_EQ(-100.0f, negated.x());
    EXPECT_FLOAT_EQ(-200.0f, negated.y());

    float value = b * g;

    EXPECT_FLOAT_EQ(5000.0f, value);
}

TEST(FloatPoint, Equality)
{
    CyberCore::FloatPoint a(100.0f, 200.0f);
    CyberCore::FloatPoint b(100.0f, 200.0f);
    CyberCore::FloatPoint c(10.0f, 20.0f);

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a == c);
    ASSERT_FALSE(a != b);
    ASSERT_TRUE(a != c);

    ASSERT_TRUE(CyberCore::areEssentiallyEqual(a, b));
    ASSERT_FALSE(CyberCore::areEssentiallyEqual(a, c));
}

TEST(FloatPoint, Floors)
{
    CyberCore::FloatPoint a(100.6f, 199.9f);

    CyberCore::IntSize flooredSize = CyberCore::flooredIntSize(a);
    EXPECT_FLOAT_EQ(100, flooredSize.width());
    EXPECT_FLOAT_EQ(199, flooredSize.height());

    CyberCore::IntPoint flooredPoint = CyberCore::flooredIntPoint(a);
    EXPECT_FLOAT_EQ(100, flooredPoint.x());
    EXPECT_FLOAT_EQ(199, flooredPoint.y());

    CyberCore::FloatPoint flooredPoint1x = CyberCore::floorPointToDevicePixels(a, 1.0);
    EXPECT_FLOAT_EQ(100.0f, flooredPoint1x.x());
    EXPECT_FLOAT_EQ(199.0f, flooredPoint1x.y());

    CyberCore::FloatPoint flooredPoint2x = CyberCore::floorPointToDevicePixels(a, 2.0);
    EXPECT_FLOAT_EQ(100.5f, flooredPoint2x.x());
    EXPECT_FLOAT_EQ(199.5f, flooredPoint2x.y());
}

TEST(FloatPoint, Rounding)
{
    CyberCore::FloatPoint a(100.4f, 199.9f);

    CyberCore::IntPoint roundedPoint = CyberCore::roundedIntPoint(a);
    EXPECT_FLOAT_EQ(100, roundedPoint.x());
    EXPECT_FLOAT_EQ(200, roundedPoint.y());
}

TEST(FloatPoint, Ceiling)
{
    CyberCore::FloatPoint a(100.4f, 199.9f);

    CyberCore::IntPoint ceilingPoint = CyberCore::ceiledIntPoint(a);
    EXPECT_FLOAT_EQ(101, ceilingPoint.x());
    EXPECT_FLOAT_EQ(200, ceilingPoint.y());

    CyberCore::FloatPoint ceilingPoint1x = CyberCore::ceilPointToDevicePixels(a, 1.0);
    EXPECT_FLOAT_EQ(101.0f, ceilingPoint1x.x());
    EXPECT_FLOAT_EQ(200.0f, ceilingPoint1x.y());

    CyberCore::FloatPoint ceilingPoint2x = CyberCore::ceilPointToDevicePixels(a, 2.0);
    EXPECT_FLOAT_EQ(100.5f, ceilingPoint2x.x());
    EXPECT_FLOAT_EQ(200.0f, ceilingPoint2x.y());
}

TEST(FloatPoint, Casting)
{
    CyberCore::FloatPoint a(100.4f, 199.9f);

    CyberCore::FloatSize floatSize = CyberCore::toFloatSize(a);
    EXPECT_FLOAT_EQ(100.4f, floatSize.width());
    EXPECT_FLOAT_EQ(199.9f, floatSize.height());

    CyberCore::FloatSize b(99.6f, 299.1f);

    CyberCore::FloatPoint floatPoint = CyberCore::toFloatPoint(b);
    EXPECT_FLOAT_EQ(99.6f, floatPoint.x());
    EXPECT_FLOAT_EQ(299.1f, floatPoint.y());

#if USE(CG)
    CGPoint cgPoint = a;

    EXPECT_FLOAT_EQ(100.4f, cgPoint.x);
    EXPECT_FLOAT_EQ(199.9f, cgPoint.y);

    CGPoint cgPoint2 = CGPointMake(-22.3f, 14.2f);

    CyberCore::FloatPoint testCG(cgPoint2);

    EXPECT_FLOAT_EQ(-22.3f, testCG.x());
    EXPECT_FLOAT_EQ(14.2f, testCG.y());
#endif
}

}
