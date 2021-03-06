include = ["core/shaders/common.shader"]

render_states = {
	debug_line = {
		rgb_write_enable = true
		alpha_write_enable = true
		depth_write_enable = true
		depth_enable = true
		depth_func = "lequal"
		blend_enable = true
		blend_src = "src_alpha"
		blend_dst = "inv_src_alpha"
		primitive_type = "pt_lines"
		cull_mode = "cw"
	}

	debug_line_noz = {
		rgb_write_enable = true
		alpha_write_enable = true
		depth_write_enable = true
		depth_enable = false
		depth_func = "lequal"
		blend_enable = true
		blend_src = "src_alpha"
		blend_dst = "inv_src_alpha"
		primitive_type = "pt_lines"
		cull_mode = "cw"
	}

	gui = {
		rgb_write_enable = true
		alpha_write_enable = true
		depth_write_enable = false
		depth_enable = false
		depth_func = "lequal"
		blend_enable = true
		blend_src = "src_alpha"
		blend_dst = "inv_src_alpha"
		cull_mode = "cw"
	}

	sprite = {
		rgb_write_enable = true
		alpha_write_enable = true
		depth_write_enable = true
		depth_enable = true
		depth_func = "always"
		blend_enable = true
		blend_src = "src_alpha"
		blend_dst = "inv_src_alpha"
		blend_equation = "add"
		cull_mode = "cw"
	}

	mesh = {
		rgb_write_enable = true
		alpha_write_enable = true
		depth_write_enable = true
		depth_enable = true
		depth_func = "lequal"
		blend_enable = false
		cull_mode = "cw"
	}
}

bgfx_shaders = {
	debug_line = {
		includes = "common"

		varying = """
			vec4 v_color0   : COLOR0 = vec4(0.0, 0.0, 0.0, 0.0);

			vec3 a_position : POSITION;
			vec4 a_color0   : COLOR0;
		"""

		vs_input_output = """
			$input a_position, a_color0
			$output v_color0
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
				v_color0 = toLinearAccurate(a_color0);
			}
		"""

		fs_input_output = """
			$input v_color0
		"""

		fs_code = """
			void main()
			{
				gl_FragColor = v_color0;
			}
		"""
	}

	gui = {
		includes = "common"

		samplers = {
			u_albedo = { sampler_state = "clamp_point" }
		}

		varying = """
			vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);
			vec4 v_color0    : COLOR0 = vec4(0.0, 0.0, 0.0, 0.0);

			vec3 a_position  : POSITION;
			vec2 a_texcoord0 : TEXCOORD0;
			vec4 a_color0    : COLOR0;
		"""

		vs_input_output = """
			$input a_position, a_texcoord0, a_color0
			$output v_texcoord0, v_color0
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
		#ifdef DIFFUSE_MAP
				v_texcoord0 = a_texcoord0;
		#endif // DIFFUSE_MAP
				v_color0 = toLinearAccurate(a_color0);
			}
		"""

		fs_input_output = """
			$input v_texcoord0, v_color0
		"""

		fs_code = """
		#ifdef DIFFUSE_MAP
			SAMPLER2D(u_albedo, 0);
		#endif // DIFFUSE_MAP

			void main()
			{
		#ifdef DIFFUSE_MAP
				gl_FragColor = texture2D(u_albedo, v_texcoord0) * v_color0;
		#else
				gl_FragColor = v_color0;
		#endif // DIFFUSE_MAP
			}
		"""
	}

	sprite = {
		includes = "common"

		samplers = {
			u_albedo = { sampler_state = "clamp_point" }
		}

		varying = """
			vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);

			vec3 a_position  : POSITION;
			vec2 a_texcoord0 : TEXCOORD0;
		"""

		vs_input_output = """
			$input a_position, a_texcoord0
			$output v_texcoord0
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
				v_texcoord0 = a_texcoord0;
			}
		"""

		fs_input_output = """
			$input v_texcoord0
		"""

		fs_code = """
			uniform vec4 u_color;
			SAMPLER2D(u_albedo, 0);

			void main()
			{
				gl_FragColor = texture2D(u_albedo, v_texcoord0) * toLinearAccurate(u_color);
			}
		"""
	}

	mesh = {
		includes = "common"

		samplers = {
			u_albedo = { sampler_state = "mirror_anisotropic" }
		}

		varying = """
			vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 0.0);
			vec4 v_view      : TEXCOORD0 = vec4(0.0, 0.0, 0.0, 0.0);
			vec2 v_texcoord0 : TEXCOORD1 = vec2(0.0, 0.0);

			vec3 a_position  : POSITION;
			vec3 a_normal    : NORMAL;
			vec2 a_texcoord0 : TEXCOORD0;
		"""

		vs_input_output = """
			$input a_position, a_normal, a_texcoord0
			$output v_normal, v_view, v_texcoord0
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
				v_view = mul(u_modelView, vec4(a_position, 1.0));
				v_normal = normalize(mul(u_modelView, vec4(a_normal, 0.0)).xyz);

				v_texcoord0 = a_texcoord0;
			}
		"""

		fs_input_output = """
			$input v_normal, v_view, v_texcoord0
		"""

		fs_code = """
		#if !defined(NO_LIGHT)
			uniform vec4 u_light_position;  // In world-space
			uniform vec4 u_light_direction; // In view-space
			uniform vec4 u_light_color;
			uniform vec4 u_light_range;
			uniform vec4 u_light_intensity;

			uniform vec4 u_ambient;
			uniform vec4 u_diffuse;
			uniform vec4 u_specular;
		#endif

		#ifdef DIFFUSE_MAP
			SAMPLER2D(u_albedo, 0);
		#endif // DIFFUSE_MAP

			void main()
			{
		#if !defined(NO_LIGHT)
				// normalize both input vectors
				vec3 n = normalize(v_normal);
				vec3 e = normalize(v_view.xyz);
				vec3 l = u_light_direction.xyz;

				float nl = max(0.0, dot(n, l));
				vec4 light_diffuse = nl * toLinearAccurate(u_light_color) * u_light_intensity.x;

				vec4 color = max(u_diffuse * light_diffuse, u_ambient);
		#else
				vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		#endif // !defined(NO_LIGHT)

		#ifdef DIFFUSE_MAP
				gl_FragColor = color * texture2D(u_albedo, v_texcoord0);
		#else
				gl_FragColor = color;
		#endif // DIFFUSE_MAP
			}
		"""
	}

	ocornut_imgui = {
		includes = "common"

		varying = """
			vec4 v_color0    : COLOR0    = vec4(1.0, 0.0, 0.0, 1.0);
			vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 1.0);
			vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);

			vec3 a_position  : POSITION;
			vec4 a_normal    : NORMAL;
			vec4 a_color0    : COLOR0;
			vec2 a_texcoord0 : TEXCOORD0;
		"""

		vs_input_output = """
			$input a_position, a_texcoord0, a_color0
			$output v_color0, v_texcoord0
		"""

		vs_code = """
			void main()
			{
				vec2 pos = 2.0*a_position.xy*u_viewTexel.xy;
				gl_Position = vec4(pos.x - 1.0, 1.0 - pos.y, 0.0, 1.0);
				v_texcoord0 = a_texcoord0;
				v_color0    = a_color0;
			}
		"""

		fs_input_output = """
			$input v_color0, v_texcoord0
		"""

		fs_code = """
			SAMPLER2D(s_tex, 0);

			void main()
			{
				vec4 texel = texture2D(s_tex, v_texcoord0);
				gl_FragColor = texel * v_color0;
			}
		"""
	}

	imgui_image = {
		includes = "common"

		varying = """
			vec4 v_color0    : COLOR0    = vec4(1.0, 0.0, 0.0, 1.0);
			vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 1.0);
			vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);

			vec3 a_position  : POSITION;
			vec4 a_normal    : NORMAL;
			vec4 a_color0    : COLOR0;
			vec2 a_texcoord0 : TEXCOORD0;
		"""

		vs_input_output = """
			$input a_position, a_texcoord0
			$output v_texcoord0
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_viewProj, vec4(a_position.xy, 0.0, 1.0) );
				v_texcoord0 = a_texcoord0;
			}
		"""

		fs_input_output = """
			$input v_texcoord0
		"""

		fs_code = """
			uniform vec4 u_imageLodEnabled;
			SAMPLER2D(s_texColor, 0);

			#define u_imageLod     u_imageLodEnabled.x
			#define u_imageEnabled u_imageLodEnabled.y

			void main()
			{
				vec3 color = texture2DLod(s_texColor, v_texcoord0, u_imageLod).xyz;
				float alpha = 0.2 + 0.8*u_imageEnabled;
				gl_FragColor = vec4(color, alpha);
			}
		"""
	}

	blit = {
		includes = "common"

		varying = """
			vec2 v_texcoord0 : TEXCOORD0 = vec2(0.0, 0.0);

			vec3 a_position  : POSITION;
			vec2 a_texcoord0 : TEXCOORD0;
		"""

		vs_input_output = """
			$input a_position, a_texcoord0
			$output v_texcoord0
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_viewProj, vec4(a_position.xy, 0.0, 1.0) );
				v_texcoord0 = a_texcoord0;
			}
		"""

		fs_input_output = """
			$input v_texcoord0
		"""

		fs_code = """
			SAMPLER2D(s_texColor, 0);

			void main()
			{
				gl_FragColor.rgb = toGammaAccurate(texture2D(s_texColor, v_texcoord0).rgb);
			}
		"""
	}

	fallback = {
		includes = "common"

		varying = """
			vec3 a_position  : POSITION;
		"""

		vs_input_output = """
			$input a_position
		"""

		vs_code = """
			void main()
			{
				gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
			}
		"""

		fs_input_output = """
		"""

		fs_code = """
			void main()
			{
				gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
			}
		"""
	}

}

shaders = {
	debug_line = {
		bgfx_shader = "debug_line"
		render_state = "debug_line"
	}

	debug_line_noz = {
		bgfx_shader = "debug_line"
		render_state = "debug_line_noz"
	}

	gui = {
		bgfx_shader = "gui"
		render_state = "gui"
	}

	sprite = {
		bgfx_shader = "sprite"
		render_state = "sprite"
	}

	mesh = {
		bgfx_shader = "mesh"
		render_state = "mesh"
	}

	ocornut_imgui = {
		bgfx_shader = "ocornut_imgui"
		render_state = "gui"
	}

	imgui_image = {
		bgfx_shader = "imgui_image"
		render_state = "gui"
	}

	blit = {
		bgfx_shader = "blit"
		render_state = "gui"
	}

	fallback = {
		bgfx_shader = "fallback"
		render_state = "mesh"
	}

}

static_compile = [
	{ shader = "debug_line" defines = [] }
	{ shader = "debug_line_noz" defines = [] }
	{ shader = "gui" defines = [] }
	{ shader = "gui" defines = ["DIFFUSE_MAP"]}
	{ shader = "sprite" defines = [] }
	{ shader = "mesh" defines = [] }
	{ shader = "mesh" defines = ["DIFFUSE_MAP"] }
	{ shader = "mesh" defines = ["DIFFUSE_MAP" "NO_LIGHT"] }
	{ shader = "ocornut_imgui" defines = [] }
	{ shader = "imgui_image" defines = [] }
	{ shader = "blit" defines = [] }
	{ shader = "fallback" defines = [] }

]
