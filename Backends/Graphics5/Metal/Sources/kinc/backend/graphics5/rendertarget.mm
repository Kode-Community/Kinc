#include <kinc/graphics5/rendertarget.h>

#include <kinc/graphics5/graphics.h>
#include <kinc/graphics5/rendertarget.h>

#import <Metal/Metal.h>

id getMetalDevice();
id getMetalEncoder();

static MTLPixelFormat convert_format(kinc_g5_render_target_format_t format) {
	switch (format) {
	case KINC_G5_RENDER_TARGET_FORMAT_128BIT_FLOAT:
		return MTLPixelFormatRGBA32Float;
	case KINC_G5_RENDER_TARGET_FORMAT_64BIT_FLOAT:
		return MTLPixelFormatRGBA16Float;
	case KINC_G5_RENDER_TARGET_FORMAT_32BIT_RED_FLOAT:
		return MTLPixelFormatR32Float;
	case KINC_G5_RENDER_TARGET_FORMAT_16BIT_RED_FLOAT:
		return MTLPixelFormatR16Float;
	case KINC_G5_RENDER_TARGET_FORMAT_8BIT_RED:
		return MTLPixelFormatR8Unorm;
	case KINC_G5_RENDER_TARGET_FORMAT_32BIT:
	default:
		return MTLPixelFormatBGRA8Unorm;
	}
}

void kinc_g5_render_target_init(kinc_g5_render_target_t *target, int width, int height, int depthBufferBits, bool antialiasing,
								kinc_g5_render_target_format_t format, int stencilBufferBits, int contextId) {
	memset(target, 0, sizeof(kinc_g5_render_target_t));

	target->texWidth = width;
	target->texHeight = height;

	target->contextId = contextId;

	id<MTLDevice> device = getMetalDevice();

	MTLTextureDescriptor* descriptor = [MTLTextureDescriptor new];
	descriptor.textureType = MTLTextureType2D;
	descriptor.width = width;
	descriptor.height = height;
	descriptor.depth = 1;
	descriptor.pixelFormat = convert_format(format);
	descriptor.arrayLength = 1;
	descriptor.mipmapLevelCount = 1;
	descriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
	descriptor.resourceOptions = MTLResourceStorageModePrivate;

	target->impl._tex = [device newTextureWithDescriptor:descriptor];

	if (depthBufferBits > 0) {
		MTLTextureDescriptor* depthDescriptor = [MTLTextureDescriptor new];
		depthDescriptor.textureType = MTLTextureType2D;
		depthDescriptor.width = width;
		depthDescriptor.height = height;
		depthDescriptor.depth = 1;
		depthDescriptor.pixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		depthDescriptor.arrayLength = 1;
		depthDescriptor.mipmapLevelCount = 1;
		depthDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
		depthDescriptor.resourceOptions = MTLResourceStorageModePrivate;

		target->impl._depthTex = [device newTextureWithDescriptor:depthDescriptor];
	}
}

void kinc_g5_render_target_init_cube(kinc_g5_render_target_t *target, int cubeMapSize, int depthBufferBits, bool antialiasing,
									 kinc_g5_render_target_format_t format, int stencilBufferBits, int contextId) {
	target->impl._tex = nil;
	target->impl._depthTex = nil;
}

void kinc_g5_render_target_destroy(kinc_g5_render_target_t *target) {
	target->impl._tex = nil;
	target->impl._depthTex = nil;
}

#if 0
void kinc_g5_set_render_target_descriptor(kinc_g5_render_target_t *renderTarget, kinc_g5_texture_descriptor_t descriptor) {
    MTLSamplerDescriptor* desc = (MTLSamplerDescriptor*) renderTarget->impl._samplerDesc;
    switch(descriptor.filter_minification) {
        case KINC_G5_TEXTURE_FILTER_POINT:
            desc.minFilter = MTLSamplerMinMagFilterNearest;
            break;
        default:
            desc.minFilter = MTLSamplerMinMagFilterLinear;
    }

    switch(descriptor.filter_magnification) {
        case KINC_G5_TEXTURE_FILTER_POINT:
            desc.magFilter = MTLSamplerMinMagFilterNearest;
            break;
        default:
            desc.minFilter = MTLSamplerMinMagFilterLinear;
    }

    switch(descriptor.addressing_u) {
        case KINC_G5_TEXTURE_ADDRESSING_REPEAT:
            desc.sAddressMode = MTLSamplerAddressModeRepeat;
            break;
        case KINC_G5_TEXTURE_ADDRESSING_MIRROR:
            desc.sAddressMode = MTLSamplerAddressModeMirrorRepeat;
            break;
        case KINC_G5_TEXTURE_ADDRESSING_CLAMP:
            desc.sAddressMode = MTLSamplerAddressModeClampToEdge;
            break;
        case KINC_G5_TEXTURE_ADDRESSING_BORDER:
            desc.sAddressMode = MTLSamplerAddressModeClampToBorderColor;
            break;
    }

    switch(descriptor.addressing_v) {
        case KINC_G5_TEXTURE_ADDRESSING_REPEAT:
            desc.tAddressMode = MTLSamplerAddressModeRepeat;
            break;
        case KINC_G5_TEXTURE_ADDRESSING_MIRROR:
            desc.tAddressMode = MTLSamplerAddressModeMirrorRepeat;
            break;
        case KINC_G5_TEXTURE_ADDRESSING_CLAMP:
            desc.tAddressMode = MTLSamplerAddressModeClampToEdge;
            break;
        case KINC_G5_TEXTURE_ADDRESSING_BORDER:
            desc.tAddressMode = MTLSamplerAddressModeClampToBorderColor;
            break;
    }
    id<MTLDevice> device = getMetalDevice();
    renderTarget->impl._sampler = [device newSamplerStateWithDescriptor:desc];
}
#endif

extern void kinc_internal_set_vertex_sampler(id encoder, int unit);
extern void kinc_internal_set_fragment_sampler(id encoder, int unit);

void kinc_g5_render_target_use_color_as_texture(kinc_g5_render_target_t *target, kinc_g5_texture_unit_t unit) {
	id<MTLRenderCommandEncoder> encoder = getMetalEncoder();
	if (unit.impl.vertex) {
		if (unit.impl.index < 16) {
			kinc_internal_set_vertex_sampler(encoder, unit.impl.index);
		}
		[encoder setVertexTexture:target->impl._tex atIndex:unit.impl.index];
	}
	else {
		if (unit.impl.index < 16) {
			kinc_internal_set_fragment_sampler(encoder, unit.impl.index);
		}
		[encoder setFragmentTexture:target->impl._tex atIndex:unit.impl.index];
	}
}

void kinc_g5_render_target_use_depth_as_texture(kinc_g5_render_target_t *target, kinc_g5_texture_unit_t unit) {
	id<MTLRenderCommandEncoder> encoder = getMetalEncoder();
	if (unit.impl.vertex) {
		if (unit.impl.index < 16) {
			kinc_internal_set_vertex_sampler(encoder, unit.impl.index);
		}
		[encoder setVertexTexture:target->impl._depthTex atIndex:unit.impl.index];
	}
	else {
		if (unit.impl.index < 16) {
			kinc_internal_set_fragment_sampler(encoder, unit.impl.index);
		}
		[encoder setFragmentTexture:target->impl._depthTex atIndex:unit.impl.index];
	}
}

void kinc_g5_render_target_set_depth_stencil_from(kinc_g5_render_target_t *target, kinc_g5_render_target_t *source) {
	target->impl._depthTex = source->impl._depthTex;
}
