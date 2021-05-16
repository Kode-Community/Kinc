#import "GLView.h"

#include <kinc/input/acceleration.h>
#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/input/rotation.h>
#include <kinc/input/surface.h>
#include <kinc/input/pen.h>
#include <kinc/system.h>
#include <kinc/graphics5/graphics.h>
#include <kinc/graphics5/rendertarget.h>

#ifdef KORE_OPENGL
#include <kinc/backend/graphics4/OpenGLWindow.h>
#endif

namespace {
	const int touchmaxcount = 20;
	void* touches[touchmaxcount];

	void initTouches() {
		for (int i = 0; i < touchmaxcount; ++i) {
			touches[i] = nullptr;
		}
	}

	int getTouchIndex(void* touch) {
		for (int i = 0; i < touchmaxcount; ++i) {
			if (touches[i] == touch) return i;
		}
		return -1;
	}

	int addTouch(void* touch) {
		for (int i = 0; i < touchmaxcount; ++i) {
			if (touches[i] == nullptr) {
				touches[i] = touch;
				return i;
			}
		}
		return -1;
	}

	int removeTouch(void* touch) {
		for (int i = 0; i < touchmaxcount; ++i) {
			if (touches[i] == touch) {
				touches[i] = nullptr;
				return i;
			}
		}
		return -1;
	}

	GLint backingWidth, backingHeight;
}

extern "C" int kinc_window_width(int window) {
	return backingWidth;
}

extern "C" int kinc_window_height(int window) {
	return backingHeight;
}

@implementation GLView

#ifdef KORE_METAL
+ (Class)layerClass {
	return [CAMetalLayer class];
}
#else
+ (Class)layerClass {
	return [CAEAGLLayer class];
}
#endif

#ifdef KORE_METAL
void initMetalCompute(id<MTLDevice> device, id<MTLCommandQueue> commandQueue);

- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:(CGRect)frame];
	self.contentScaleFactor = [UIScreen mainScreen].scale;

	backingWidth = frame.size.width * self.contentScaleFactor;
	backingHeight = frame.size.height * self.contentScaleFactor;

	initTouches();

	device = MTLCreateSystemDefaultDevice();
	commandQueue = [device newCommandQueue];
	library = [device newDefaultLibrary];
	initMetalCompute(device, commandQueue);

	CAMetalLayer* metalLayer = (CAMetalLayer*)self.layer;

	metalLayer.device = device;
	metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
	metalLayer.framebufferOnly = YES;
	// metalLayer.presentsWithTransaction = YES;

	metalLayer.opaque = YES;
	metalLayer.backgroundColor = nil;

	return self;
}
#else
- (id)initWithFrame:(CGRect)frame {
	self = [super initWithFrame:(CGRect)frame];
	self.contentScaleFactor = [UIScreen mainScreen].scale;

	backingWidth = frame.size.width * self.contentScaleFactor;
	backingHeight = frame.size.height * self.contentScaleFactor;

	initTouches();

	CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;

	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
	                                                                          kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

	if (!context || ![EAGLContext setCurrentContext:context]) {
		//[self release];
		return nil;
	}

	glGenFramebuffersOES(1, &defaultFramebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
	Kinc_Internal_windows[0].framebuffer = defaultFramebuffer;

	glGenRenderbuffersOES(1, &colorRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);

	glGenRenderbuffersOES(1, &depthStencilRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthStencilRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthStencilRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthStencilRenderbuffer);

	// Start acceletometer
	hasAccelerometer = false;
#ifndef KORE_TVOS
	motionManager = [[CMMotionManager alloc] init];
	if ([motionManager isAccelerometerAvailable]) {
		motionManager.accelerometerUpdateInterval = 0.033;
		[motionManager startAccelerometerUpdates];
		hasAccelerometer = true;
	}
#endif

#ifndef KORE_TVOS
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onKeyboardHide:) name:UIKeyboardWillHideNotification object:nil];
#endif

	return self;
}
#endif

#ifdef KORE_METAL
- (void)begin {
}
#else
- (void)begin {
	[EAGLContext setCurrentContext:context];
	//glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
	//glViewport(0, 0, backingWidth, backingHeight);

#ifndef KORE_TVOS
	// Accelerometer updates
	if (hasAccelerometer) {

		CMAcceleration acc = motionManager.accelerometerData.acceleration;

		if (acc.x != lastAccelerometerX || acc.y != lastAccelerometerY || acc.z != lastAccelerometerZ) {

			if (kinc_acceleration_callback != NULL) {
				(*kinc_acceleration_callback)(acc.x, acc.y, acc.z);
			}

			lastAccelerometerX = acc.x;
			lastAccelerometerY = acc.y;
			lastAccelerometerZ = acc.z;
		}
	}
#endif
}
#endif

#ifdef KORE_METAL
- (void)end {
}
#else
- (void)end {
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES]; // crash at end
}
#endif

#ifdef KORE_METAL
- (void)layoutSubviews {
}
#else
- (void)layoutSubviews {
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];

	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);

	printf("backingWitdh/Height: %i, %i\n", backingWidth, backingHeight);

	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthStencilRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH24_STENCIL8_OES, backingWidth, backingHeight);

	if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
	}
}
#endif

#ifdef KORE_METAL
- (void)dealloc {
}
#else
- (void)dealloc {
	if (defaultFramebuffer) {
		glDeleteFramebuffersOES(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}

	if (colorRenderbuffer) {
		glDeleteRenderbuffersOES(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}

	if (depthStencilRenderbuffer) {
		glDeleteRenderbuffersOES(1, &depthStencilRenderbuffer);
		depthStencilRenderbuffer = 0;
	}

	if ([EAGLContext currentContext] == context) [EAGLContext setCurrentContext:nil];

	//[context release];
	context = nil;

	//[super dealloc];
}
#endif

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
	for (UITouch* touch in touches) {
		int index = getTouchIndex((__bridge void*)touch);
		if (index == -1) index = addTouch((__bridge void*)touch);
		if (index >= 0) {
			CGPoint point = [touch locationInView:self];
			float x = point.x * self.contentScaleFactor;
			float y = point.y * self.contentScaleFactor;
			if (index == 0) {
				kinc_internal_mouse_trigger_press(0, 0, x, y);
			}
			kinc_internal_surface_trigger_touch_start(index, x, y);

			if (touch.type == UITouchTypePencil) {
				kinc_internal_pen_trigger_press(0, x, y, 0.0);
			}
		}
	}
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
	for (UITouch* touch in touches) {
		int index = getTouchIndex((__bridge void*)touch);
		if (index >= 0) {
			CGPoint point = [touch locationInView:self];
			float x = point.x * self.contentScaleFactor;
			float y = point.y * self.contentScaleFactor;
			if (index == 0) {
				kinc_internal_mouse_trigger_move(0, x, y);
			}
			kinc_internal_surface_trigger_move(index, x, y);
		}
	}
}

- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches {
	for (UITouch* touch in touches) {
		if (touch.type == UITouchTypePencil) {
			CGPoint point = [touch locationInView:self];
			float x = point.x * self.contentScaleFactor;
			float y = point.y * self.contentScaleFactor;
			kinc_internal_pen_trigger_move(0, x, y, touch.force);
		}
	}
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
	for (UITouch* touch in touches) {
		int index = removeTouch((__bridge void*)touch);
		if (index >= 0) {
			CGPoint point = [touch locationInView:self];
			float x = point.x * self.contentScaleFactor;
			float y = point.y * self.contentScaleFactor;
			if (index == 0) {
				kinc_internal_mouse_trigger_release(0, 0, x, y);
			}
			kinc_internal_surface_trigger_touch_end(index, x, y);

			if (touch.type == UITouchTypePencil) {
				kinc_internal_pen_trigger_release(0, x, y, 0.0);
			}
		}
	}
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event {
	for (UITouch* touch in touches) {
		int index = removeTouch((__bridge void*)touch);
		if (index >= 0) {
			CGPoint point = [touch locationInView:self];
			float x = point.x * self.contentScaleFactor;
			float y = point.y * self.contentScaleFactor;
			if (index == 0) {
				kinc_internal_mouse_trigger_release(0, 0, x, y);
			}
			kinc_internal_surface_trigger_touch_end(index, x, y);

			if (touch.type == UITouchTypePencil) {
				kinc_internal_pen_trigger_release(0, x, y, 0.0);
			}
		}
	}
}

namespace {
	NSString* keyboardstring;
	UITextField* myTextField = nullptr;
	bool shiftDown = false;
}

- (void)showKeyboard {
	[self becomeFirstResponder];
}

- (void)hideKeyboard {
	[self resignFirstResponder];
}

- (BOOL)hasText {
	return YES;
}

- (void)insertText:(NSString*)text {
	if ([text length] == 1) {
		unichar ch = [text characterAtIndex:[text length] - 1];
		if (ch == 8212) ch = '_';
		if (ch == L'\n') {
			kinc_internal_keyboard_trigger_key_down(KINC_KEY_RETURN);
			kinc_internal_keyboard_trigger_key_up(KINC_KEY_RETURN);
		}
		else if (ch >= L'a' && ch <= L'z') {
			if (shiftDown) {
				kinc_internal_keyboard_trigger_key_up(KINC_KEY_SHIFT);
				shiftDown = false;
			}
			kinc_internal_keyboard_trigger_key_down(ch + KINC_KEY_A - L'a');
			kinc_internal_keyboard_trigger_key_up(ch + KINC_KEY_A - L'a');
			kinc_internal_keyboard_trigger_key_press(ch);
		}
		else {
			if (!shiftDown) {
				kinc_internal_keyboard_trigger_key_down(KINC_KEY_SHIFT);
				shiftDown = true;
			}
			kinc_internal_keyboard_trigger_key_down(ch + KINC_KEY_A - L'A');
			kinc_internal_keyboard_trigger_key_up(ch + KINC_KEY_A - L'A');
			kinc_internal_keyboard_trigger_key_press(ch);
		}
	}
}

- (void)deleteBackward {
	kinc_internal_keyboard_trigger_key_down(KINC_KEY_BACKSPACE);
	kinc_internal_keyboard_trigger_key_up(KINC_KEY_BACKSPACE);
}

- (BOOL)canBecomeFirstResponder {
	return YES;
}

- (void)onKeyboardHide:(NSNotification*)notification {
	kinc_keyboard_hide();
}

#ifdef KORE_METAL
- (CAMetalLayer*)metalLayer {
	return (CAMetalLayer*)self.layer;
}

- (id<MTLDevice>)metalDevice {
	return device;
}

- (id<MTLLibrary>)metalLibrary {
	return library;
}

- (id<MTLCommandQueue>)metalQueue {
	return commandQueue;
}
#endif

@end
