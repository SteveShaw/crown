/*
 * Copyright (c) 2012-2015 Daniele Bartolini and individual contributors.
 * License: https://github.com/taylor001/crown/blob/master/LICENSE
 */

#include "debug_line.h"
#include "math_utils.h"
#include "color4.h"
#include "vector3.h"
#include "matrix4x4.h"
#include "config.h"
#include <string.h>
#include <bgfx.h>

namespace crown
{

namespace debug_line
{
#if CROWN_PLATFORM_LINUX || CROWN_PLATFORM_ANDROID
	static const uint8_t vs_h[335] =
	{
		0x56, 0x53, 0x48, 0x03, 0x1c, 0xf0, 0xa8, 0xc9, 0x01, 0x00, 0x0f, 0x75, 0x5f, 0x6d, 0x6f, 0x64, // VSH........u_mod
		0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x09, 0x01, 0x00, 0x00, 0x01, 0x00, // elViewProj......
		0x2a, 0x01, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x6d, 0x65, // *...attribute me
		0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, // diump vec4 a_col
		0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x6d, // or0;.attribute m
		0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x61, 0x5f, 0x70, 0x6f, // ediump vec3 a_po
		0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, // sition;.varying
		0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x76, 0x5f, 0x63, // mediump vec4 v_c
		0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x6d, // olor0;.uniform m
		0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, // ediump mat4 u_mo
		0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, // delViewProj;.voi
		0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x6d, 0x65, // d main ().{.  me
		0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // diump vec4 tmpva
		0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, // r_1;.  tmpvar_1.
		0x77, 0x20, 0x3d, 0x20, 0x31, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // w = 1.0;.  tmpva
		0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, // r_1.xyz = a_posi
		0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, // tion;.  gl_Posit
		0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, // ion = (u_modelVi
		0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // ewProj * tmpvar_
		0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x20, 0x3d, // 1);.  v_color0 =
		0x20, 0x61, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,       //  a_color0;.}...
	};

	static const uint8_t fs_h[91] =
	{
		0x46, 0x53, 0x48, 0x03, 0x1c, 0xf0, 0xa8, 0xc9, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x76, 0x61, // FSH.......L...va
		0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x6d, 0x65, 0x64, 0x69, 0x75, 0x6d, 0x70, 0x20, 0x76, 0x65, // rying mediump ve
		0x63, 0x34, 0x20, 0x76, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x76, 0x6f, 0x69, // c4 v_color0;.voi
		0x64, 0x20, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x67, 0x6c, // d main ().{.  gl
		0x5f, 0x46, 0x72, 0x61, 0x67, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x76, 0x5f, 0x63, // _FragColor = v_c
		0x6f, 0x6c, 0x6f, 0x72, 0x30, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                               // olor0;.}...
	};
#elif CROWN_PLATFORM_WINDOWS
	static const uint8_t vs_h[419] =
	{
		0x56, 0x53, 0x48, 0x03, 0xa4, 0x8b, 0xef, 0x49, 0x01, 0x00, 0x0f, 0x75, 0x5f, 0x6d, 0x6f, 0x64, // VSH....I...u_mod
		0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x09, 0x01, 0x00, 0x00, 0x04, 0x00, // elViewProj......
		0x80, 0x01, 0x00, 0x03, 0xfe, 0xff, 0xfe, 0xff, 0x23, 0x00, 0x43, 0x54, 0x41, 0x42, 0x1c, 0x00, // ........#.CTAB..
		0x00, 0x00, 0x57, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0xff, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, // ..W.............
		0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x02, 0x00, // ......P...0.....
		0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x5f, // ......@.......u_
		0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x00, 0x03, 0x00, // modelViewProj...
		0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x73, // ..............vs
		0x5f, 0x33, 0x5f, 0x30, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74, 0x20, 0x28, // _3_0.Microsoft (
		0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, // R) HLSL Shader C
		0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x39, 0x2e, 0x32, 0x39, 0x2e, 0x39, 0x35, 0x32, // ompiler 9.29.952
		0x2e, 0x33, 0x31, 0x31, 0x31, 0x00, 0x51, 0x00, 0x00, 0x05, 0x04, 0x00, 0x0f, 0xa0, 0x00, 0x00, // .3111.Q.........
		0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, // .?..............
		0x00, 0x02, 0x0a, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0f, 0x90, 0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, // ................
		0x00, 0x80, 0x01, 0x00, 0x0f, 0x90, 0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, // ................
		0x0f, 0xe0, 0x1f, 0x00, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x80, 0x01, 0x00, 0x0f, 0xe0, 0x05, 0x00, // ................
		0x00, 0x03, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0x00, 0x90, 0x05, 0x00, // ................
		0x00, 0x03, 0x01, 0x00, 0x0f, 0x80, 0x01, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0x55, 0x90, 0x02, 0x00, // ............U...
		0x00, 0x03, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0x80, 0x01, 0x00, 0xe4, 0x80, 0x05, 0x00, // ................
		0x00, 0x03, 0x01, 0x00, 0x0f, 0x80, 0x02, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0xaa, 0x90, 0x02, 0x00, // ................
		0x00, 0x03, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0x80, 0x01, 0x00, 0xe4, 0x80, 0x01, 0x00, // ................
		0x00, 0x02, 0x01, 0x00, 0x0f, 0x80, 0x03, 0x00, 0xe4, 0xa0, 0x05, 0x00, 0x00, 0x03, 0x01, 0x00, // ................
		0x0f, 0x80, 0x01, 0x00, 0xe4, 0x80, 0x04, 0x00, 0x00, 0xa0, 0x02, 0x00, 0x00, 0x03, 0x00, 0x00, // ................
		0x0f, 0x80, 0x00, 0x00, 0xe4, 0x80, 0x01, 0x00, 0xe4, 0x80, 0x01, 0x00, 0x00, 0x02, 0x01, 0x00, // ................
		0x0f, 0x80, 0x00, 0x00, 0xe4, 0x90, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, // ................
		0xe4, 0x80, 0x01, 0x00, 0x00, 0x02, 0x01, 0x00, 0x0f, 0xe0, 0x01, 0x00, 0xe4, 0x80, 0xff, 0xff, // ................
		0x00, 0x00, 0x00,                                                                               // ...
	};

	static const uint8_t fs_h[137] =
	{
		0x46, 0x53, 0x48, 0x03, 0xa4, 0x8b, 0xef, 0x49, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x03, 0xff, 0xff, // FSH....I..|.....
		0xfe, 0xff, 0x16, 0x00, 0x43, 0x54, 0x41, 0x42, 0x1c, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, // ....CTAB....#...
		0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, // ................
		0x1c, 0x00, 0x00, 0x00, 0x70, 0x73, 0x5f, 0x33, 0x5f, 0x30, 0x00, 0x4d, 0x69, 0x63, 0x72, 0x6f, // ....ps_3_0.Micro
		0x73, 0x6f, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53, 0x4c, 0x20, 0x53, 0x68, // soft (R) HLSL Sh
		0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65, 0x72, 0x20, 0x39, 0x2e, // ader Compiler 9.
		0x32, 0x39, 0x2e, 0x39, 0x35, 0x32, 0x2e, 0x33, 0x31, 0x31, 0x31, 0x00, 0x1f, 0x00, 0x00, 0x02, // 29.952.3111.....
		0x0a, 0x00, 0x00, 0x80, 0x00, 0x00, 0x0f, 0x90, 0x01, 0x00, 0x00, 0x02, 0x00, 0x08, 0x0f, 0x80, // ................
		0x00, 0x00, 0xe4, 0x90, 0xff, 0xff, 0x00, 0x00, 0x00,                                           // .........
	};
#endif

	static bgfx::VertexDecl s_decl;
	static bgfx::ProgramHandle s_prog;

	void init()
	{
		s_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
			.end();

		bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_h, sizeof(vs_h)));
		CE_ASSERT(bgfx::isValid(vs), "Failed to create vertex shader");
		bgfx::ShaderHandle fs = bgfx::createShader(bgfx::makeRef(fs_h, sizeof(fs_h)));
		CE_ASSERT(bgfx::isValid(fs), "Failed to create fragment shader");
		s_prog = bgfx::createProgram(vs, fs, true);
		CE_ASSERT(bgfx::isValid(s_prog), "Failed to create program");
	}

	void shutdown()
	{
		bgfx::destroyProgram(s_prog);
	}

} // namespace debug_line

DebugLine::DebugLine(bool depth_test)
	: _depth_test(depth_test)
	, _num(0)
{
}

void DebugLine::add_line(const Vector3& start, const Vector3& end, const Color4& color)
{
	if (_num >= CROWN_MAX_DEBUG_LINES)
		 return;

	_lines[_num].p0[0] = start.x;
	_lines[_num].p0[1] = start.y;
	_lines[_num].p0[2] = start.z;
	_lines[_num].c0    = color4::to_abgr(color);

	_lines[_num].p1[0] = end.x;
	_lines[_num].p1[1] = end.y;
	_lines[_num].p1[2] = end.z;
	_lines[_num].c1    = color4::to_abgr(color);

	_num++;
}

void DebugLine::add_sphere(const Vector3& center, const float radius, const Color4& color)
{
	const uint32_t deg_step = 15;

	for (uint32_t deg = 0; deg < 360; deg += deg_step)
	{
		const float rad0 = to_rad((float) deg);
		const float rad1 = to_rad((float) deg + deg_step);

		// XZ plane
		const Vector3 start0(cos(rad0) * radius, 0, -sin(rad0) * radius);
		const Vector3 end0  (cos(rad1) * radius, 0, -sin(rad1) * radius);
		add_line(center + start0, center + end0, color);

		// XY plane
		const Vector3 start1(cos(rad0) * radius, sin(rad0) * radius, 0);
		const Vector3 end1  (cos(rad1) * radius, sin(rad1) * radius, 0);
		add_line(center + start1, center + end1, color);

		// YZ plane
		const Vector3 start2(0, sin(rad0) * radius, -cos(rad0) * radius);
		const Vector3 end2  (0, sin(rad1) * radius, -cos(rad1) * radius);
		add_line(center + start2, center + end2, color);
	}
}

void DebugLine::add_obb(const Matrix4x4& tm, const Vector3& extents, const Color4& color)
{
	const Vector3 o = Vector3(tm.t.x, tm.t.y, tm.t.z);
	const Vector3 x = Vector3(tm.x.x, tm.x.y, tm.x.z) * (extents.x * 0.5f);
	const Vector3 y = Vector3(tm.y.x, tm.y.y, tm.y.z) * (extents.y * 0.5f);
	const Vector3 z = Vector3(tm.z.x, tm.z.y, tm.z.z) * (extents.z * 0.5f);

	// Back face
	add_line(o - x - y - z, o + x - y - z, color);
	add_line(o + x - y - z, o + x + y - z, color);
	add_line(o + x + y - z, o - x + y - z, color);
	add_line(o - x + y - z, o - x - y - z, color);

	add_line(o - x - y + z, o + x - y + z, color);
	add_line(o + x - y + z, o + x + y + z, color);
	add_line(o + x + y + z, o - x + y + z, color);
	add_line(o - x + y + z, o - x - y + z, color);

	add_line(o - x - y - z, o - x - y + z, color);
	add_line(o + x - y - z, o + x - y + z, color);
	add_line(o + x + y - z, o + x + y + z, color);
	add_line(o - x + y - z, o - x + y + z, color);
}

void DebugLine::clear()
{
	_num = 0;
}

void DebugLine::commit()
{
	if (!_num)
		return;

	if (!checkAvailTransientVertexBuffer(_num * 2, debug_line::s_decl))
		return;

	bgfx::TransientVertexBuffer tvb;
	bgfx::allocTransientVertexBuffer(&tvb, _num * 2, debug_line::s_decl);

	memcpy(tvb.data, _lines, sizeof(Line) * _num);

	bgfx::setState(BGFX_STATE_PT_LINES
		| BGFX_STATE_RGB_WRITE
		| (_depth_test ? (BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_DEPTH_WRITE) : 0)
		| BGFX_STATE_CULL_CW);

	bgfx::setProgram(debug_line::s_prog);
	bgfx::setVertexBuffer(&tvb, 0, _num * 2);
	bgfx::submit(0);
}

} // namespace crown
