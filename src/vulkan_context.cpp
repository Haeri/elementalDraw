#include "../include/vulkan_context.hpp"

VulkanContext::VulkanContext()
{
}

VulkanContext::~VulkanContext()
{
}

Context* VulkanContext::create_context()
{
	return nullptr;
}

void VulkanContext::delete_context()
{
}

void VulkanContext::stroke_rect(float x, float y, float width, float height)
{
}

void VulkanContext::stroke_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl)
{
}

void VulkanContext::stroke_line(float x, float y)
{
}

void VulkanContext::stroke_circle(float x, float y, float radius)
{
}

void VulkanContext::stroke_ellipse(float x, float y, float width, float height)
{
}

void VulkanContext::stroke_polygon(float x, float y)
{
}

void VulkanContext::fill_rect(float x, float y, float width, float height)
{
}

void VulkanContext::fill_rounded_rect(float x, float y, float width, float height, float tl, float tr, float br, float bl)
{
}

void VulkanContext::fill_circle(float x, float y, float radius)
{
}

void VulkanContext::fill_ellipse(float x, float y, float width, float height)
{
}

void VulkanContext::fill_polygon(float x, float y)
{
}

void VulkanContext::draw_text(float x, float y, char* text)
{
}

void VulkanContext::draw_image(float x, float y, float width, float height, uint32_t image)
{
}

void VulkanContext::draw_rounded_image(float x, float y, float width, float height, uint32_t image, float tl, float tr, float br, float bl)
{
}

void VulkanContext::set_color(Color color)
{
}

void VulkanContext::set_line_width(float width)
{
}

void VulkanContext::clear()
{
}

void VulkanContext::clear_rect(float x, float y, float width, float height)
{
}
