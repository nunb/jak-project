#pragma once

#include "common/math/Vector.h"
#include "game/graphics/opengl_renderer/BucketRenderer.h"

struct TfragRenderSettings {
  math::Matrix4f math_camera;
  math::Vector4f hvdf_offset;
  float fog_x;
  int tree_idx;
  float time_of_day_weights[8] = {0};
  math::Vector4f planes[4];
  bool debug_culling = false;
  const u8* occlusion_culling = nullptr;
  // todo occlusion culling string.
};

enum class DoubleDrawKind { NONE, AFAIL_NO_DEPTH_WRITE };

struct DoubleDraw {
  DoubleDrawKind kind = DoubleDrawKind::NONE;
  float aref_first = 0.;
  float aref_second = 0.;
};

DoubleDraw setup_tfrag_shader(SharedRenderState* render_state, DrawMode mode);
DoubleDraw setup_opengl_from_draw_mode(DrawMode mode, u32 tex_unit, bool mipmap);

void first_tfrag_draw_setup(const TfragRenderSettings& settings, SharedRenderState* render_state);
void interp_time_of_day_slow(const float weights[8],
                             const std::vector<tfrag3::TimeOfDayColor>& in,
                             math::Vector<u8, 4>* out);

struct SwizzledTimeOfDay {
  std::vector<u8> data;
  u32 color_count = 0;
};

SwizzledTimeOfDay swizzle_time_of_day(const std::vector<tfrag3::TimeOfDayColor>& in);

void interp_time_of_day_fast(const float weights[8],
                             const SwizzledTimeOfDay& in,
                             math::Vector<u8, 4>* out);

void cull_check_all_slow(const math::Vector4f* planes,
                         const std::vector<tfrag3::VisNode>& nodes,
                         const u8* level_occlusion_string,
                         u8* out);
bool sphere_in_view_ref(const math::Vector4f& sphere, const math::Vector4f* planes);

struct TfragPcPortData {
  math::Vector4f planes[4];
  math::Vector<s32, 4> itimes[4];
  math::Vector4f camera[4];
  math::Vector4f hvdf_off;
  float fogx;
  float unused[3];
  char level_name[12];
  u32 tree_idx;
};

u32 make_index_list_from_vis_string(std::pair<int, int>* group_out,
                                    u32* idx_out,
                                    const std::vector<tfrag3::StripDraw>& draws,
                                    const std::vector<u8>& vis_data);
u32 make_all_visible_index_list(std::pair<int, int>* group_out,
                                u32* idx_out,
                                const std::vector<tfrag3::StripDraw>& draws);