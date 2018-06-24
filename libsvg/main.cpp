#include <getopt.h>

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <svg.h>

#define LOG_ERR(T) std::cerr << __FUNCTION__ << ":" << std::to_string(__LINE__) << ":" << T << std::endl
#define LOG_TEXT(T) std::cout << __FUNCTION__ << ":" << std::to_string(__LINE__) << ":" << T << std::endl

#if 0
#  define LOG_TRACE std::cout << __FUNCTION__ << ":" << std::to_string(__LINE__) << std::endl
#else
#  define LOG_TRACE
#endif

struct SBSVGRenderer
{
  std::string pos;
  std::string valueFrom;
  std::string cpoint1;
  std::string cpoint2;
  std::string valueTo;

  std::string target;
  std::vector<std::string> playlist;
};

static svg_status_t t_begin_group(void *closure, double opacity)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_begin_element(void *closure)
{
  LOG_TRACE;

  SBSVGRenderer * renderer = (SBSVGRenderer *)closure;

  renderer->pos = "";
  renderer->valueFrom = "";
  renderer->cpoint1 = "";
  renderer->cpoint2 = "";
  renderer->valueTo = "";

  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_end_element(void *closure)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_end_group(void *closure, double opacity)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_move_to(void *closure, double x, double y)
{
  LOG_TRACE;

  SBSVGRenderer * renderer = (SBSVGRenderer *)closure;

  renderer->pos = std::to_string(x) + "," + std::to_string(y);

  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_line_to(void *closure, double x, double y)
{
  LOG_TRACE;

  SBSVGRenderer * renderer = (SBSVGRenderer *)closure;

  renderer->valueFrom = renderer->pos;
  renderer->valueTo = std::to_string(x) + "," + std::to_string(y);
  renderer->pos = renderer->valueTo;

  const std::string animator = "line (p1,p2) : " + renderer->valueFrom + renderer->valueTo;
  renderer->playlist.push_back(std::move(animator));

  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_curve_to(void *closure,
  double x1, double y1,
  double x2, double y2,
  double x3, double y3)
{
  LOG_TRACE;

  SBSVGRenderer * renderer = (SBSVGRenderer *)closure;

  renderer->valueFrom = renderer->pos;
  renderer->cpoint1 = std::to_string(x1) + "," + std::to_string(y1);
  renderer->cpoint2 = std::to_string(x2) + "," + std::to_string(y2);
  renderer->valueTo = std::to_string(x3) + "," + std::to_string(y3);
  renderer->pos = renderer->valueTo;

  const std::string animator = "curve (p1,cp1,cp2,p2) : " + renderer->valueFrom + " -> " + renderer->cpoint1 + " -> " + renderer->cpoint2 + " -> " + renderer->valueTo;
  renderer->playlist.push_back(std::move(animator));

  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_quadratic_curve_to(void *closure,
  double x1, double y1,
  double x2, double y2)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_arc_to(void *closure,
  double     rx,
  double     ry,
  double     x_axis_rotation,
  int        large_arc_flag,
  int        sweep_flag,
  double     x,
  double     y)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_close_path(void *closure)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_color(void *closure, const svg_color_t *color)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_fill_opacity(void *closure, double fill_opacity)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_fill_paint(void *closure, const svg_paint_t *paint)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_fill_rule(void *closure, svg_fill_rule_t fill_rule)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_font_family(void *closure, const char *family)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_font_size(void *closure, double size)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_font_style(void *closure, svg_font_style_t font_style)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_font_weight(void *closure, unsigned int font_weight)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_opacity(void *closure, double opacity)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_dash_array(void *closure, double *dash_array, int num_dashes)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_dash_offset(void *closure, svg_length_t *offset)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_line_cap(void *closure, svg_stroke_line_cap_t line_cap)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_line_join(void *closure, svg_stroke_line_join_t line_join)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_miter_limit(void *closure, double limit)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_opacity(void *closure, double stroke_opacity)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_paint(void *closure, const svg_paint_t *paint)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_stroke_width(void *closure, svg_length_t *width)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_text_anchor(void *closure, svg_text_anchor_t text_anchor)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_transform(void *closure,
  double a, double b,
  double c, double d,
  double e, double f)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_apply_view_box(void *closure,
  svg_view_box_t view_box,
  svg_length_t *width,
  svg_length_t *height)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_set_viewport_dimension(void *closure,
  svg_length_t *width,
  svg_length_t *height)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_render_line(void *closure,
  svg_length_t *x1,
  svg_length_t *y1,
  svg_length_t *x2,
  svg_length_t *y2)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_render_path(void *closure)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_render_ellipse(void *closure,
  svg_length_t *cx,
  svg_length_t *cy,
  svg_length_t *rx,
  svg_length_t *ry)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_render_rect(void *closure,
  svg_length_t *x,
  svg_length_t *y,
  svg_length_t *width,
  svg_length_t *height,
  svg_length_t *rx,
  svg_length_t *ry)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_render_text(void *closure,
  svg_length_t *x,
  svg_length_t *y,
  const char   *utf8)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_status_t t_render_image(void *closure,
  unsigned char *data,
  unsigned int  data_width,
  unsigned int  data_height,
  svg_length_t  *x,
  svg_length_t  *y,
  svg_length_t  *width,
  svg_length_t  *height)
{
  LOG_TRACE;
  return SVG_STATUS_SUCCESS;
}

static svg_render_engine_t t_render_engine = {
    /* hierarchy */
    t_begin_group,
    t_begin_element,
    t_end_element,
    t_end_group,
    /* path creation */
    t_move_to,
    t_line_to,
    t_curve_to,
    t_quadratic_curve_to,
    t_arc_to,
    t_close_path,
    /* style */
    t_set_color,
    t_set_fill_opacity,
    t_set_fill_paint,
    t_set_fill_rule,
    t_set_font_family,
    t_set_font_size,
    t_set_font_style,
    t_set_font_weight,
    t_set_opacity,
    t_set_stroke_dash_array,
    t_set_stroke_dash_offset,
    t_set_stroke_line_cap,
    t_set_stroke_line_join,
    t_set_stroke_miter_limit,
    t_set_stroke_opacity,
    t_set_stroke_paint,
    t_set_stroke_width,
    t_set_text_anchor,
    /* transform */
    t_transform,
    t_apply_view_box,
    t_set_viewport_dimension,
    /* drawing */
    t_render_line,
    t_render_path,
    t_render_ellipse,
    t_render_rect,
    t_render_text,
    t_render_image
};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "./0 <svgpath>" << std::endl;
    return EXIT_FAILURE;
  }
  
  const std::string svgpath = argv[1];

  if (svgpath.empty())
  {
    std::cerr << "./0 <svgpath>" << std::endl;
    return EXIT_FAILURE;
  }

  SBSVGRenderer renderer;

  svg_status_t ret = SVG_STATUS_SUCCESS;

  svg_t * svg = NULL;

  if ((ret = svg_create(&svg)) != SVG_STATUS_SUCCESS)
  {
    LOG_ERR("error : svg_create");
  }

  if ((ret = svg_parse(svg, svgpath.c_str())) != SVG_STATUS_SUCCESS)
  {
    LOG_ERR("error : svg_parse");
  }

  if ((ret = svg_render(svg, &t_render_engine, &renderer)) != SVG_STATUS_SUCCESS)
  {
    LOG_ERR("error : " + std::to_string(ret));
  }

  if ((ret = svg_destroy(svg)) != SVG_STATUS_SUCCESS)
  {
    LOG_ERR("error : svg_destroy");
  }

  std::stringstream ss;

  for (const std::string& curentry : renderer.playlist)
  {
    ss << "\t\t\t\t" << curentry << std::endl;
  }

  std::cout << ss.str();

  return 0;
}

