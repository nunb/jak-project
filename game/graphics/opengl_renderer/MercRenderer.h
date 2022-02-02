#pragma once

#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "common/math/Vector.h"

#include "game/graphics/opengl_renderer/DirectRenderer.h"

enum class Mask {
  NONE = 0,
  x = 1,
  y = 2,
  xy = 3,
  z = 4,
  xz = 5,
  yz = 6,
  xyz = 7,
  w = 8,
  xw = 9,
  yw = 10,
  xyw = 11,
  zw = 12,
  xzw = 13,
  yzw = 14,
  xyzw = 15
};

inline float vu_max(float a, float b) {
  s32 ai, bi;
  memcpy(&ai, &a, 4);
  memcpy(&bi, &b, 4);
  if (ai > bi) {
    return a;
  } else {
    return b;
  }
}

inline float vu_min(float a, float b) {
  s32 ai, bi;
  memcpy(&ai, &a, 4);
  memcpy(&bi, &b, 4);
  if (ai > bi) {
    return b;
  } else {
    return a;
  }
}

struct Vf {
  float data[4];
  float& x() { return data[0]; }
  float& y() { return data[1]; }
  float& z() { return data[2]; }
  float& w() { return data[3]; }

  const float& x() const { return data[0]; }
  const float& y() const { return data[1]; }
  const float& z() const { return data[2]; }
  const float& w() const { return data[3]; }

  u16 x_as_u16() const {
    u16 result;
    memcpy(&result, &data[0], 2);
    return result;
  }

  u16 y_as_u16() const {
    u16 result;
    memcpy(&result, &data[1], 2);
    return result;
  }

  u16 z_as_u16() const {
    u16 result;
    memcpy(&result, &data[2], 2);
    return result;
  }

  u16 w_as_u16() const {
    u16 result;
    memcpy(&result, &data[3], 2);
    return result;
  }

  u32 x_as_u32() const {
    u32 result;
    memcpy(&result, &data[0], 4);
    return result;
  }

  u32 y_as_u32() const {
    u32 result;
    memcpy(&result, &data[1], 4);
    return result;
  }

  u32 z_as_u32() const {
    u32 result;
    memcpy(&result, &data[2], 4);
    return result;
  }

  u32 w_as_u32() const {
    u32 result;
    memcpy(&result, &data[3], 4);
    return result;
  }

  Vf() = default;
  Vf(const math::Vector4f& vec) { memcpy(data, vec.data(), 16); }
  Vf(float a, float b, float c, float d) {
    data[0] = a;
    data[1] = b;
    data[2] = c;
    data[3] = d;
  }

  std::string str_float() const { return fmt::format("{} {} {} {}", x(), y(), z(), w()); }

  float& operator[](int i) { return data[i]; }
  float operator[](int i) const { return data[i]; }

  void mr32(Mask mask, const Vf& other) {
    float temp[4];
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        temp[i] = other[(i + 1) % 4];
      }
    }

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = temp[i];
      }
    }
  }

  void mfir(Mask mask, s16 in) {
    s32 sext = in;
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        memcpy(data + i, &sext, 4);
      }
    }
  }

  void maxi(Mask mask, const Vf& other, float I) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = vu_max(other[i], I);
      }
    }
  }

  void max(Mask mask, const Vf& other, float I) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = vu_max(other[i], I);
      }
    }
  }

  void max(Mask mask, const Vf& other, const Vf& b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = vu_max(other[i], b[i]);
      }
    }
  }

  void minii(Mask mask, const Vf& other, float I) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = vu_min(other[i], I);
      }
    }
  }

  void mini(Mask mask, const Vf& other, float I) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = vu_min(other[i], I);
      }
    }
  }

  void move(Mask mask, const Vf& other) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = other[i];
      }
    }
  }

  void mfp(Mask mask, float other) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = other;
      }
    }
  }

  void add(Mask mask, const Vf& a, const Vf& b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] + b[i];
      }
    }
  }

  void add(Mask mask, const Vf& a, float b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] + b;
      }
    }
  }

  void mul(Mask mask, const Vf& a, const Vf& b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] * b[i];
      }
    }
  }

  void mul(Mask mask, const Vf& a, float b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] * b;
      }
    }
  }

  void itof0(Mask mask, const Vf& a) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        s32 val;
        memcpy(&val, &a.data[i], 4);
        data[i] = val;
      }
    }
  }

  void itof15(Mask mask, const Vf& a) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        s32 val;
        memcpy(&val, &a.data[i], 4);
        data[i] = ((float)val) * (1.f / 32768.f);
        ;
      }
    }
  }

  void ftoi4(Mask mask, const Vf& a) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        s32 val = a.data[i] * 16.f;
        memcpy(&data[i], &val, 4);
      }
    }
  }

  void ftoi0(Mask mask, const Vf& a) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        s32 val = a.data[i];
        memcpy(&data[i], &val, 4);
      }
    }
  }
};

struct Accumulator {
  float data[4];

  void adda(Mask mask, const Vf& a, float b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] + b;
      }
    }
  }

  void madda(Mask mask, const Vf& a, const Vf& b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] += a[i] * b[i];
      }
    }
  }

  void madda(Mask mask, const Vf& a, float b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] += a[i] * b;
      }
    }
  }

  void madd(Mask mask, Vf& dest, const Vf& a, const Vf& b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        dest[i] = data[i] + a[i] * b[i];
      }
    }
  }

  void madd(Mask mask, Vf& dest, const Vf& a, float b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        dest[i] = data[i] + a[i] * b;
      }
    }
  }

  void mula(Mask mask, const Vf& a, const Vf& b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] * b[i];
      }
    }
  }

  void mula(Mask mask, const Vf& a, float b) {
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        data[i] = a[i] * b;
      }
    }
  }
};

class MercRenderer : public BucketRenderer {
 public:
  MercRenderer(const std::string& name, BucketId my_id);
  void render(DmaFollower& dma, SharedRenderState* render_state, ScopedProfilerNode& prof) override;
  void draw_debug_window() override;

 private:
  void handle_setup(DmaFollower& dma, SharedRenderState* render_state, ScopedProfilerNode& prof);
  void handle_merc_chain(DmaFollower& dma,
                         SharedRenderState* render_state,
                         ScopedProfilerNode& prof);
  void unpack8(const VifCodeUnpack& up, const u8* data, u32 imm);
  void unpack32(const VifCodeUnpack& up, const u8* data, u32 imm);

  void mscal(int enter_address, SharedRenderState* render_state, ScopedProfilerNode& prof);
  void xgkick(u16 addr, SharedRenderState* render_state, ScopedProfilerNode& prof);

  enum MercDataMemory {
    LOW_MEMORY = 0,
    BUFFER_BASE = 442,
    // this negative offset is what broke jak graphics in Dobiestation for a long time.
    BUFFER_OFFSET = -442
  };

  struct LowMemory {
    u8 tri_strip_tag[16];
    u8 ad_gif_tag[16];
    math::Vector4f hvdf_offset;
    math::Vector4f perspective[4];
    math::Vector4f fog;
  } m_low_memory;
  static_assert(sizeof(LowMemory) == 0x80);

  u8 m_buffer_memory[(1024) * 16];

  void sq_buffer(Mask mask, const Vf& data, u32 qw) {
    //    if (data.x_as_u32() == 0x80000000 && data.y_as_u32() == 0x80000000) {
    //      fmt::print("big store line {}: {} : {} {} {} {}\n", line_number, qw, data.x(), data.y(),
    //      data.z(), data.w());
    //    }
    // sketchy...
    //    qw &= 1023;
    assert(qw * 16 < sizeof(m_buffer_memory));
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        memcpy(m_buffer_memory + qw * 16 + i * 4, data.data + i, 4);
      }
    }
  }

  void lq_buffer(Mask mask, Vf& dest, u16 addr);

  void isw_buffer(Mask mask, u16 val, u16 addr);
  void ilw_buffer(Mask mask, u16& dest, u16 addr);

  u16 xtop();
  u16 xitop();

  DirectRenderer m_direct;

  struct {
    u32 row[4] = {0, 0, 0, 0};
    bool stmod = false;

  } m_vif;

  struct Stats {
    int unpack_count = 0;
    int unpack_bytes = 0;
    int mscal_35 = 0;
    int mscal_20 = 0;
    int mscal_17 = 0;
    int mscal_32 = 0;
    bool had_data = false;
    std::string str;
  } m_stats;

  bool m_dbf = false;

  bool m_enable_prime = true;
  bool m_force_prime = false;


  struct Vu {
    Vf vf01, vf02, vf03, vf04, vf05, vf06, vf07, vf08, vf09, vf10, vf11, vf12, vf13, vf14, vf15,
        vf16, vf17, vf18, vf19, vf20, vf21, vf22, vf23, vf24, vf25, vf26, vf27, vf28, vf29, vf30,
        vf31;
    const Vf vf00;
    u16 vi01, vi02, vi03, vi04, vi05, vi06, vi07, vi09, vi08, vi11, vi12, vi13, vi10, vi14, vi15;
    float I, P, Q;

    Accumulator acc;
    const u16 vi00 = 0;

    u16 hack_old_vi15 = 0;

    Vu() : vf00(0, 0, 0, 1) {}
  } vu;
};
