// Copyright (c) 2012, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include "embedders/openglui/common/canvas_state.h"

#include <ctype.h>
#include <string.h>

#include "embedders/openglui/common/support.h"

// Float parser. Does not handle exponents. This is
// used to parse font sizes so it doesn't need to be full-blown.
float ParseFloat(const char* s, int& pos) {
  int len = strlen(s);
  float rtn = 0.0;
  while (pos < len && s[pos] != '.' && !isdigit(s[pos])) {
    ++pos;
  }
  while (pos < len && isdigit(s[pos])) {
    rtn = rtn * 10.0 + s[pos] - '0';
    ++pos;
  }
  if (pos < len && s[pos] == '.') {
    pos++;
    float div = 1.0;
    while (pos < len && isdigit(s[pos])) {
      rtn = rtn * 10.0 + s[pos] - '0';
      ++pos;
      div *= 10.0;
    }
    rtn /= div;
  }
  return rtn;
}

int ParseInt(const char* s, int& pos) {
  int len = strlen(s);
  int rtn = 0;
  while (pos < len && !isdigit(s[pos])) {
    ++pos;
  }
  while (pos < len && isdigit(s[pos])) {
    rtn = rtn * 10 + s[pos] - '0';
    ++pos;
  }
  return rtn;
}

void CanvasState::setLineCap(const char* lc) {
  if (strcmp(lc, "round") == 0) {
    paint_.setStrokeCap(SkPaint::kRound_Cap);
  } else if (strcmp(lc, "square") == 0) {
    paint_.setStrokeCap(SkPaint::kSquare_Cap);
  } else {
    paint_.setStrokeCap(SkPaint::kButt_Cap);
  }
}

void CanvasState::setLineJoin(const char* lj) {
  if (strcmp(lj, "round") == 0) {
    paint_.setStrokeJoin(SkPaint::kRound_Join);
  } else if (strcmp(lj, "bevel") == 0) {
    paint_.setStrokeJoin(SkPaint::kBevel_Join);
  } else {
    paint_.setStrokeJoin(SkPaint::kMiter_Join);
  }
}

const char* CanvasState::setFont(const char*name, float size) {
  // Font names have the form "<modifier> <size> <family>".
  // Modifiers are "normal", "italic", "bold".
  // Sizes have magnitude and units; e.g. "10pt", "20px".
  const char* rtn = name;
  if (size < 0) {
    int pos = 0;
    // TODO(gram): need to handle these modifiers.
    if (strncmp(name, "normal", 6) == 0) {
      pos = 6;
    } else if (strncmp(name, "italic", 6) == 0) {
      pos = 6;
    } else if (strncmp(name, "bold", 4) == 0) {
      pos = 4;
    }
    size = ParseFloat(name, pos);
    // Font size units: px, etc. For now just px.
    // TODO(gram): Handle other units.
    if (strncmp(name + pos, "px", 2) == 0) {
      pos += 2;
    }
    int len = strlen(name);
    while (pos < len && isspace(name[pos])) {
      ++pos;
    }
    name = name + pos;
  }
  SkTypeface *pTypeface = SkTypeface::CreateFromName(name, SkTypeface::kNormal);
  paint_.setTypeface(pTypeface);
  paint_.setTextSize(size);
  pTypeface->unref();
  // TODO(gram): Must return a normalized font name incorporating size, so
  // callers can set the Dart canvas font name to an appropriate value.
  // Actually this may not be necessary in which case we can change this
  // method to return void.
  return rtn;
}

const char* CanvasState::setTextAlign(const char* align) {
  // TODO(gram): What about "start" and "end"?
  // I don't see any an analogs in Skia.
  if (strcmp(align, "left") == 0) {
    paint_.setTextAlign(SkPaint::kLeft_Align);
  } else if (strcmp(align, "right") == 0) {
    paint_.setTextAlign(SkPaint::kRight_Align);
  } else {
    paint_.setTextAlign(SkPaint::kCenter_Align);
  }
  return align;
}

const char* CanvasState::setTextBaseline(const char* baseline) {
  // TODO(gram): Does skia support this? It doesn't seem like
  // it. Right now we don't use the textBaseline value, but we
  // may have to implement this ourselves, by adjusting the y
  // values passed to StrokeText/FillText using the font metrics.
  if (strcmp(baseline, "top") == 0) {
  } else if (strcmp(baseline, "middle") == 0) {
  } else if (strcmp(baseline, "bottom") == 0) {
  } else if (strcmp(baseline, "hanging") == 0) {
  } else if (strcmp(baseline, "alphabetic") == 0) {
  } else if (strcmp(baseline, "ideographic") == 0) {
  }
  return baseline;
}

const char* CanvasState::setTextDirection(const char* direction) {
    // TODO(gram): Add support for this if Skia does.
  return direction;
}

void CanvasState::setGlobalCompositeOperation(const char* op) {
  SkXfermode::Mode mode;
  if (strcmp(op, "source-atop") == 0) {
    mode = SkXfermode::kSrcATop_Mode;
  } else if (strcmp(op, "source-in") == 0) {
    mode = SkXfermode::kSrcIn_Mode;
  } else if (strcmp(op, "source-out") == 0) {
    mode = SkXfermode::kSrcOut_Mode;
  } else if (strcmp(op, "source-over") == 0) {
    mode = SkXfermode::kSrcOver_Mode;  // Default.
  } else if (strcmp(op, "destination-atop") == 0) {
    mode = SkXfermode::kDstATop_Mode;
  } else if (strcmp(op, "destination-in") == 0) {
    mode = SkXfermode::kDstIn_Mode;
  } else if (strcmp(op, "destination-out") == 0) {
    mode = SkXfermode::kDstOut_Mode;
  } else if (strcmp(op, "destination-over") == 0) {
    mode = SkXfermode::kDstOver_Mode;
  } else if (strcmp(op, "lighter") == 0) {
    mode = SkXfermode::kLighten_Mode;
  } else if (strcmp(op, "darker") == 0) {
    mode = SkXfermode::kDarken_Mode;
  } else if (strcmp(op, "xor") == 0) {
    mode = SkXfermode::kXor_Mode;
  } else if (strcmp(op, "copy") == 0) {
    mode = SkXfermode::kSrc_Mode;
  }
  SkXfermode* m = SkXfermode::Create(mode);
  // It seems we don't need unref() here. Including it causes
  // a crash. Maybe Skia has a preallocated long-lived set of
  // instances.
  paint_.setXfermode(m);
}

void CanvasState::Arc(float x, float y, float radius,
                      float startAngle, float endAngle,
                      bool antiClockwise) {
  SkRect rect;
  rect.set(x - radius, y - radius, x + radius, y + radius);
  bool doCircle = false;

  static float twoPi = 2 * M_PI;

  float sweep = endAngle - startAngle;
  if (sweep >= twoPi || sweep <= -twoPi) {
    doCircle = true;
  }

  if (!antiClockwise && endAngle <= startAngle) {
    endAngle += 2 * M_PI;
  } else if (antiClockwise && startAngle <= endAngle) {
    startAngle += 2 * M_PI;
  }
  sweep = endAngle - startAngle;

  startAngle = fmodf(startAngle, twoPi);
  float sa = Radians2Degrees(startAngle);
  float ea = Radians2Degrees(sweep);
  path_->arcTo(rect, sa, ea, false);
  if (doCircle) {
    SkPath tmp;
    tmp.addOval(rect);
    tmp.addPath(*path_);
    path_->swap(tmp);
  }
}

int hexDigit(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c <= 'Z') return c - 'A' + 10;
  return c - 'a' + 10;
}

// Color parser.
// See http://www.w3.org/TR/CSS21/syndata.html#color-units.
// There is also another format: hsl(240,100%,100%) (and hsla)
// TODO(gram): We probably eventually want to use a table rather
// than a big if statement.
ColorRGBA CanvasState::GetColor(const char* color) {
  if (color[0] == '#') {
    int r, g, b;
    if (strlen(color) == 7) {
      r = hexDigit(color[1]) * 16 + hexDigit(color[2]);
      g = hexDigit(color[3]) * 16 + hexDigit(color[4]);
      b = hexDigit(color[5]) * 16 + hexDigit(color[6]);
    } else if (strlen(color) == 4) {
      r = hexDigit(color[1]) * 16 + hexDigit(color[1]);
      g = hexDigit(color[2]) * 16 + hexDigit(color[2]);
      b = hexDigit(color[3]) * 16 + hexDigit(color[3]);
    }
    return ColorRGBA(r, g, b);
  } else if (strcmp(color, "maroon") == 0) {
    return ColorRGBA(0x80, 0x00, 0x00);
  } else if (strcmp(color, "red") == 0) {
    return ColorRGBA(0xFF, 0x00, 0x00);
  } else if (strcmp(color, "orange") == 0) {
    return ColorRGBA(0xFF, 0xA5, 0x00);
  } else if (strcmp(color, "yellow") == 0) {
    return ColorRGBA(0xFF, 0xFF, 0x00);
  } else if (strcmp(color, "olive") == 0) {
    return ColorRGBA(0x80, 0x80, 0x00);
  } else if (strcmp(color, "purple") == 0) {
    return ColorRGBA(0x80, 0x00, 0x80);
  } else if (strcmp(color, "fuschia") == 0) {
    return ColorRGBA(0xFF, 0x00, 0xFF);
  } else if (strcmp(color, "white") == 0) {
    return ColorRGBA(0xFF, 0xFF, 0xFF);
  } else if (strcmp(color, "lime") == 0) {
    return ColorRGBA(0x00, 0xFF, 0x00);
  } else if (strcmp(color, "green") == 0) {
    return ColorRGBA(0x00, 0x80, 0x00);
  } else if (strcmp(color, "navy") == 0) {
    return ColorRGBA(0x00, 0x00, 0x80);
  } else if (strcmp(color, "blue") == 0) {
    return ColorRGBA(0x00, 0x00, 0xFF);
  } else if (strcmp(color, "aqua") == 0) {
    return ColorRGBA(0x00, 0xFF, 0xFF);
  } else if (strcmp(color, "teal") == 0) {
    return ColorRGBA(0x00, 0x80, 0x80);
  } else if (strcmp(color, "silver") == 0) {
    return ColorRGBA(0xC0, 0xC0, 0xC0);
  } else if (strcmp(color, "gray") == 0) {
    return ColorRGBA(0x80, 0x80, 0x80);
  } else if (strncmp(color, "rgb(", 4) == 0) {
    int pos = 4;
    int r = ParseInt(color, pos);
    ++pos;
    int g = ParseInt(color, pos);
    ++pos;
    int b = ParseInt(color, pos);
    return ColorRGBA(r, g, b);
  } else if (strncmp(color, "rgba(", 5) == 0) {
    int pos = 5;
    int r = ParseInt(color, pos);
    ++pos;
    int g = ParseInt(color, pos);
    ++pos;
    int b = ParseInt(color, pos);
    ++pos;
    float a = ParseFloat(color, pos);
    return ColorRGBA(r, g, b, static_cast<int>(a * 255.0));
  }
  // Default to black.
  return ColorRGBA(0x00, 0x00, 0x00);
}

void CanvasState::DrawImage(const SkBitmap& bm,
                            int sx, int sy, int sw, int sh,
                            int dx, int dy, int dw, int dh) {
  if (sw < 0) sw = bm.width();
  if (dw < 0) dw = bm.width();
  if (sh < 0) sh = bm.height();
  if (dh < 0) dh = bm.height();
  SkIRect src = SkIRect::MakeXYWH(sx, sy, sw, sh);
  SkRect dst = SkRect::MakeXYWH(dx, dy, dw, dh);
  LOGI("DrawImage(_,%d,%d,%d,%d,%d,%d,%d,%d)", sx, sy, sw, sh, dx, dy, dw, dh);
  canvas_->drawBitmapRect(bm, &src, dst);
}

