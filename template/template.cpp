// Template, IGAD version 3, Raspberry PI 4 version
// Get the latest version from: https://github.com/jbikker/tmpl8pi
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#define STB_IMAGE_IMPLEMENTATION
#include "template.h"

#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include "imgui.h"

#include "game.h"
#include "imgui_impl_opengl3.h"

#include <gl2ext.h>

#include "Timer.h"
using namespace Tmpl8;


// Enable usage of dedicated GPUs in notebooks
// Note: this does cause the linker to produce a .lib and .exp file;
// see http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
#if WINBUILD
#pragma comment( linker, "/subsystem:windows /ENTRY:mainCRTStartup" )
/* extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
} */
#endif


// Get us to the correct working folder when running from vs
// ----------------------------------------------------------------------------
void FixWorkingFolder()
{
	static bool fixed = false;
	if (fixed) return;
	FILE* f = fopen("assets/font.png", "rb");
	if (f) fclose(f); /* if this worked, we're already in the right folder */ else chdir("../../..");
	fixed = true;
}

// Helper functions
// ----------------------------------------------------------------------------

bool FileExists(const char* f)
{
	ifstream s(f);
	return s.good();
}

bool RemoveFile(const char* f)
{
	if (!FileExists(f)) return false;
	return !remove(f);
}

uint FileSize(string filename)
{
	ifstream s(filename);
	return s.good();
}

string TextFileRead(const char* _File)
{
	ifstream s(_File);
	string str((istreambuf_iterator<char>(s)), istreambuf_iterator<char>());
	s.close();
	return str;
}

void FatalError(const char* fmt, ...)
{
	char t[16384];
	va_list args;
	va_start(args, fmt);
	vsnprintf(t, sizeof(t), fmt, args);
	va_end(args);
	printf(t);
	exit(0);
}

void CheckEGL(EGLBoolean result, const char* file, const uint line)
{
	if (result == EGL_TRUE) return;
	GLint error = glGetError();
	if (error == GL_INVALID_ENUM)
		FATALERROR("EGL error: invalid enum.\n%s, line %i", file, line);
	if (error == GL_INVALID_VALUE)
		FATALERROR("EGL error: invalid value.\n%s, line %i", file, line);
	if (error == GL_INVALID_OPERATION)
		FATALERROR("EGL error: invalid operation.\n%s, line %i", file, line);
	if (error == GL_OUT_OF_MEMORY)
		FATALERROR("EGL error: out of memory.\n%s, line %i", file, line);
	if (error == EGL_BAD_DISPLAY)
		FATALERROR("EGL error: bad display.\n%s, line %i", file, line);
	if (error == EGL_BAD_ATTRIBUTE)
		FATALERROR("EGL error: bad attribute.\n%s, line %i", file, line);
	if (error == EGL_NOT_INITIALIZED)
		FATALERROR("EGL error: not initialized.\n%s, line %i", file, line);
	if (error == EGL_BAD_PARAMETER)
		FATALERROR("EGL error: bad parameter.\n%s, line %i", file, line);
	FATALERROR("EGL error: unknown error.\n%s, line %i", file, line);
}

#define CHECK_EGL( x ) CheckEGL( x, __FILE__, __LINE__ )

// Minimal X11 input manager
// ----------------------------------------------------------------------------

static Display* x11Display;
static long unsigned int x11Window;
static EGLContext eglContext;
static EGLConfig eglConfig;
static EGLDisplay eglDisplay;
static EGLSurface eglSurface;
//static int* ks = 0;
//static int device = -1;

Game* game;

//declared as global
static bool should_close;

// window attributes
XWindowAttributes attributes_{};
static Atom deleteWindow = 0;

void GetMousePos(int& childx, int& childy)
{
	int rootx, rooty;
	uint mask;
	Window w1, w2;
	XQueryPointer(x11Display, x11Window, &w1, &w2, &rootx, &rooty, &childx, &childy, &mask);
}

//void* InputHandlerThread(void* x)
//{
//	//get keyboard on linux
//
//	//device = open("/dev/input/eventX", O_RDONLY);
//	device = XGrabKeyboard(x11Display, x11Window, 0, GrabModeAsync, GrabModeAsync, CurrentTime);
//	if (XGrabKeyboard(x11Display, x11Window, 0, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess)
//	{
//		fprintf(stderr, "Could not grab keyboard.\n");
//		XCloseDisplay(x11Display);
//		return 0;
//	}
//	//if (device < 0) printf("could not open keyboard.\n");
//	//else
//	//	while (1)
//	//	{
//	//		struct input_event e[64];
//	//		ssize_t eventsSize = read(device, e, sizeof(input_event) * 64);
//	//		int events = (int)(eventsSize / sizeof(input_event));
//	//		for (int i = 0; i < events; i++)
//	//		{
//	//			if (e[i].type == EV_KEY /* keyboard */)
//	//			{
//	//				if (e[i].code == BTN_LEFT)
//	//				{
//	//					/* mouse button */
//	//				}
//	//				else if (e[i].code == BTN_MIDDLE)
//	//				{
//	//					/* mouse button */
//	//				}
//	//				else if (e[i].code == BTN_RIGHT)
//	//				{
//	//					/* mouse button */
//	//				}
//	//				else if (e[i].value == 2)
//	//				{
//	//					/* ignore key repeat */
//	//				}
//	//				else if (e[i].value == 1 /* down */)
//	//					ks[e[i].code] = 1;
//	//				else if (e[i].value == 0 /* up */)
//	//					ks[e[i].code] = 0;
//	//			}
//	//			else if (e[i].type == EV_REL /* mouse; see input-event-codes.h for others */)
//	//			{
//	//				if (e[i].code == REL_WHEEL)
//	//				{
//	//					/* check e[i].value */
//	//				}
//	//			}
//	//		}
//	//	}
//	XSelectInput(x11Display, x11Window, KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
//	// Grab the pointer to hide and capture it
//	/*XGrabPointer(x11Display, x11Window, True, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync,
//	             GrabModeAsync, None, None, CurrentTime);*/
//	XEvent event;
//	KeySym key;
//	char keybuf[64];
//
//	while (true)
//	{
//		//get event
//		XNextEvent(x11Display, &event);
//		int n = XLookupString(&event.xkey, keybuf, sizeof(keybuf), &key, nullptr);
//		n;
//		if (event.type == KeyPress)
//		{
//			ks[key] = 1;
//		}
//		else if (event.type == KeyRelease)
//		{
//			ks[key] = 0;
//		}
//		else if (event.type == MotionNotify)
//		{
//			// Handle mouse motion
//
//			//printf("Mouse motion: (%d, %d)\n", game->mousePos.x, game->mousePos.y);
//		}
//
//		//	//letters
//		//	if (n > 0)
//		//	{
//		//		keybuf[n] = 0;
//
//		//		if (key == XK_Escape)
//		//		{
//		//			printf("Escape key pressed\n");
//		//		}
//		//		else
//		//			printf("Key pressed: %s\n", keybuf);
//		//	} //arrow keys
//		//	else
//		//	{
//		//		if (key == XK_Left)
//		//		{
//		//			printf("Left arrow key pressed\n");
//		//		}
//		//		else if (key == XK_Right)
//		//		{
//		//			printf("Right arrow key pressed\n");
//		//		}
//		//		else if (key == XK_Up)
//		//		{
//		//			printf("Up arrow key pressed\n");
//		//		}
//		//		else if (key == XK_Down)
//		//		{
//		//			printf("Down arrow key pressed\n");
//		//		}
//		//	}
//		//}
//		//else if (event.type == ButtonPress)
//		//{
//		//	if (event.xbutton.button == Button1)
//		//	{
//		//		//printf("Left mouse button pressed\n");
//		//	}
//		//	else if (event.xbutton.button == Button2)
//		//	{
//		//		//printf("Middle mouse button pressed\n");
//		//	}
//		//	else if (event.xbutton.button == Button3)
//		//	{
//		//		//printf("Right mouse button pressed\n");
//		//	}
//		//}
//	}
//}
//Input method modified from Lasse 220010
/// <summary>
/// Converts X11 key semantics to ImGui semantics since neither uses a conventional system
/// </summary>
/// <param name="in"></param>
/// <returns></returns>
inline ImGuiKey X11SymToImGuiKeycode(const unsigned long in)
{
	// Convert keysym to Dear ImGui key code
	switch (in)
	{
	case XK_Escape: return ImGuiKey_Escape;
	case XK_Return: return ImGuiKey_Enter;
	case XK_Tab: return ImGuiKey_Tab;
	case XK_BackSpace: return ImGuiKey_Backspace;
	case XK_Delete: return ImGuiKey_Delete;
	case XK_Left: return ImGuiKey_LeftArrow;
	case XK_Right: return ImGuiKey_RightArrow;
	case XK_Up: return ImGuiKey_UpArrow;
	case XK_Down: return ImGuiKey_DownArrow;
	case XK_Home: return ImGuiKey_Home;
	case XK_End: return ImGuiKey_End;
	case XK_Page_Up: return ImGuiKey_PageUp;
	case XK_Page_Down: return ImGuiKey_PageDown;
	case XK_Shift_L: return ImGuiKey_LeftShift;
	case XK_Shift_R: return ImGuiKey_RightShift;
	case XK_Control_L: return ImGuiKey_LeftCtrl;
	case XK_Control_R: return ImGuiKey_RightCtrl;
	case XK_Alt_L: return ImGuiKey_LeftAlt;
	case XK_Alt_R: return ImGuiKey_RightAlt;
	case XK_Super_L: return ImGuiKey_LeftSuper;
	case XK_Super_R: return ImGuiKey_RightSuper;
	case XK_space: return ImGuiKey_Space;

	// Add cases for letter keys
	case XK_a:
	case XK_A: return ImGuiKey_A;
	case XK_b:
	case XK_B: return ImGuiKey_B;
	case XK_c:
	case XK_C: return ImGuiKey_C;
	case XK_d:
	case XK_D: return ImGuiKey_D;
	case XK_e:
	case XK_E: return ImGuiKey_E;
	case XK_f:
	case XK_F: return ImGuiKey_F;
	case XK_g:
	case XK_G: return ImGuiKey_G;
	case XK_h:
	case XK_H: return ImGuiKey_H;
	case XK_i:
	case XK_I: return ImGuiKey_I;
	case XK_j:
	case XK_J: return ImGuiKey_J;
	case XK_k:
	case XK_K: return ImGuiKey_K;
	case XK_l:
	case XK_L: return ImGuiKey_L;
	case XK_m:
	case XK_M: return ImGuiKey_M;
	case XK_n:
	case XK_N: return ImGuiKey_N;
	case XK_o:
	case XK_O: return ImGuiKey_O;
	case XK_p:
	case XK_P: return ImGuiKey_P;
	case XK_q:
	case XK_Q: return ImGuiKey_Q;
	case XK_r:
	case XK_R: return ImGuiKey_R;
	case XK_s:
	case XK_S: return ImGuiKey_S;
	case XK_t:
	case XK_T: return ImGuiKey_T;
	case XK_u:
	case XK_U: return ImGuiKey_U;
	case XK_v:
	case XK_V: return ImGuiKey_V;
	case XK_w:
	case XK_W: return ImGuiKey_W;
	case XK_x:
	case XK_X: return ImGuiKey_X;
	case XK_y:
	case XK_Y: return ImGuiKey_Y;
	case XK_z:
	case XK_Z: return ImGuiKey_Z;
	case XK_0: return ImGuiKey_0;
	case XK_1: return ImGuiKey_1;
	case XK_2: return ImGuiKey_2;
	case XK_3: return ImGuiKey_3;
	case XK_4: return ImGuiKey_4;
	case XK_5: return ImGuiKey_5;
	case XK_6: return ImGuiKey_6;
	case XK_7: return ImGuiKey_7;
	case XK_8: return ImGuiKey_8;
	case XK_9: return ImGuiKey_9;
	default: return ImGuiKey_None;
	}
}

//Input method modified from Lasse 220010
void ProccessEvents(Game* game)
{
	//event to store event data in
	static XEvent event;

	// key event data
	char str[25] = {0};
	KeySym key_sym = 0;

	// mouse event data
	int x, y;
	unsigned int button;

	//see if queue is empty, if not handle events
	while (XEventsQueued(x11Display, QueuedAfterReading) > 0)
	{
		//get next event
		XNextEvent(x11Display, &event);
		switch (event.type)
		{
		// keys have been remapped
		case KeymapNotify:
			XRefreshKeyboardMapping(&event.xmapping);
			break;
		//key has been pressed
		case KeyPress:
			// get pressed key
			XLookupString(&event.xkey, str, 25, &key_sym, nullptr);
			
			// stop program if escape is pressed
			if (key_sym == XK_Escape)
			{
				should_close = true;
				return;
			}

			ImGui::GetIO().AddInputCharacter(str[0]);
			ImGui::GetIO().AddKeyEvent(X11SymToImGuiKeycode(key_sym), true);

			if (!ImGui::GetIO().WantCaptureKeyboard)
			{
				game->KeyDown(key_sym);
			}

			break;
		// key is released
		case KeyRelease:
			// get pressed key
			XLookupString(&event.xkey, str, 25, &key_sym, nullptr);

			ImGui::GetIO().AddKeyEvent(X11SymToImGuiKeycode(key_sym), false);

			if (!ImGui::GetIO().WantCaptureKeyboard)
			{
				game->KeyUp(key_sym);
			}
			break;
		// mouse button pressed
		case ButtonPress:

			// calculate which button is pressed as X11 switches middle and right click + is 1 indexed
			button = event.xbutton.button - 1;

			switch (button)
			{
			case 1:
				button = 2;
				break;
			case 2:
				button = 1;
				break;
			case 3:
				ImGui::GetIO().AddMouseWheelEvent(0, 1.0f);
				if (!ImGui::GetIO().WantCaptureMouse)
				{
					game->MouseScroll(1.0f);
				}
				return;
			case 4:
				ImGui::GetIO().AddMouseWheelEvent(0, -1.0f);
				if (!ImGui::GetIO().WantCaptureMouse)
				{
					game->MouseScroll(-1.0f);
				}
				return;
			default:
				break;
			}

			ImGui::GetIO().MouseDown[button] = true;

			if (!ImGui::GetIO().WantCaptureMouse)
			{
				game->MouseDown(button);
			}

			break;
		// mouse button released
		case ButtonRelease:
			// calculate which button is pressed as X11 switches middle and right click + is 1 indexed
			button = event.xbutton.button - 1;

			if (button == 1)
			{
				button = 2;
			}
			else if (button == 2)
			{
				button = 1;
			}

			ImGui::GetIO().MouseDown[button] = false;

			if (!ImGui::GetIO().WantCaptureMouse)
			{
				game->MouseUp(button);
			}
			break;
		// mouse moved
		case MotionNotify:
			x = event.xmotion.x;
			y = event.xmotion.y;

			ImGui::GetIO().MousePos = { static_cast<float>(x), static_cast<float>(y) };

			game->MouseMove(
				static_cast<int>(static_cast<float>(x) / static_cast<float>(attributes_.width) * static_cast<float>(
					SCRWIDTH)),
				static_cast<int>(static_cast<float>(y) / static_cast<float>(attributes_.height) * static_cast<float>(
					SCRHEIGHT)));
			break;
		// screen got resized
		case ConfigureNotify:
			XGetWindowAttributes(x11Display, event.xexpose.window, &attributes_);
			glViewport(0, 0, attributes_.width, attributes_.height);

			ImGui::GetIO().DisplaySize = ImVec2{
				static_cast<float>(attributes_.width), static_cast<float>(attributes_.height)
			};
		//DebugDrawer::SetWindowResolution({attributes_.width, attributes_.height});
			break;
		case ClientMessage:
			//// window closed
			if (static_cast<Atom>(event.xclient.data.l[0]) == deleteWindow)
			{
				should_close = true;
				return;
			}
		//window got destroyed
		case DestroyNotify:
			should_close = true;
			return;
		default:
			break;
		}
	}
}

// EGL initialization; 
// heavily based on code by Brian Beuken
// ----------------------------------------------------------------------------

void InitEGL()
{
	// open display
	if (!(x11Display = XOpenDisplay(NULL))) FatalError("Could not open display");
	x11Window = DefaultRootWindow(x11Display);
	// set window attributes
	XSetWindowAttributes windowAttributes{};
	windowAttributes.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;
	windowAttributes.background_pixmap = None;
	windowAttributes.background_pixel = 0;
	windowAttributes.border_pixel = 0;
	windowAttributes.override_redirect = true;

	// create window
	x11Window = XCreateWindow(x11Display, x11Window, 0, 0, SCRWIDTH, SCRHEIGHT, 0, CopyFromParent, InputOutput,
	                          CopyFromParent, CWEventMask, &windowAttributes);
	if (!x11Window) FatalError("Could not create window");
	// show the window
	XMapWindow(x11Display, x11Window);
	XStoreName(x11Display, x11Window, "pi4 template");
	// get EGL display
	if (!(eglDisplay = eglGetDisplay((EGLNativeDisplayType)x11Display))) FatalError("Could not get EGL display");
	// init EGL
	EGLint majorVersion = 0, minorVersion = 0;
	if (!eglInitialize(eglDisplay, &majorVersion, &minorVersion))
		FatalError(
			"Could not initialize EGL: %i", eglGetError());
	// get EGL frame buffer configs for display
	EGLint numConfigs;
	eglGetConfigs(eglDisplay, NULL, 0, &numConfigs);
	// choose EGL frame buffer configuration
	static const EGLint EGL_x11DisplayATTRIBUTE_LIST[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_CONFORMANT, EGL_OPENGL_ES3_BIT_KHR,
		EGL_SAMPLE_BUFFERS, 0, // no MSAA
		EGL_SAMPLES, 1, // or 4, for MSAA
		EGL_NONE
	};
	eglChooseConfig(eglDisplay, EGL_x11DisplayATTRIBUTE_LIST, &eglConfig, 1, &numConfigs);
	// create surface to display graphics on
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)x11Window, NULL);
	// create EGL rendering context
	static const EGLint GLES3_ATTRIBUTE_LIST[] =
	{
		EGL_CONTEXT_MAJOR_VERSION_KHR, 3,
		EGL_CONTEXT_MINOR_VERSION_KHR, 1,
		EGL_NONE, EGL_NONE
	};
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, GLES3_ATTRIBUTE_LIST);
	if (eglContext == EGL_NO_CONTEXT) FatalError("Could not create context: %i", eglGetError());
	// all done
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	printf("Initialized with major verision: %i, minor version: %i\n", majorVersion, minorVersion);
	printf("This GPU supports: %s\n", glGetString(GL_VERSION));
	// prepare egl state
	eglSwapInterval(eglDisplay, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(1);
	glDepthRangef(0.0f, 1.0f);
	glClearDepthf(1.0f);


	//set gl viewport
	XGetWindowAttributes(x11Display, x11Window, &attributes_);
	glViewport(0, 0, attributes_.width, attributes_.height);
	//set window name
	//show window
	deleteWindow = XInternAtom(x11Display, "WM_DELETE_WINDOW", False);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glCullFace(GL_BACK);
}

void closeEGL()
{
	XDestroyWindow(x11Display, x11Window);
	XFree(ScreenOfDisplay(x11Display, 0));
	XCloseDisplay(x11Display);
}

// application entry point
// ----------------------------------------------------------------------------
static Timer timer;

int main(int argc, char* argv[])
{
	//debug code
	const auto pegl_debug_message_control_khr = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKKHRPROC>(eglGetProcAddress(
		"glDebugMessageCallback"));
	if (pegl_debug_message_control_khr == nullptr)
	{
		printf("failed to eglGetProcAddress eglDebugMessageControlKHR\n");
	}
	else
	{
		const GLDEBUGPROCKHR debug_fn = [](GLenum source, GLenum type, GLuint id, const GLenum severity, GLsizei length,
		                                   const GLchar* message, const void*)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH_KHR:
			case GL_DEBUG_SEVERITY_MEDIUM_KHR:
				cout << message << std::endl;
			case GL_DEBUG_SEVERITY_LOW_KHR:
			case GL_DEBUG_SEVERITY_NOTIFICATION_KHR:
			default:
				break; //Ignore.
			}
		};
		pegl_debug_message_control_khr(debug_fn, nullptr);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR);
	}

	//multi X init
	//XInitThreads();

	setenv("DISPLAY", ":0", 1);
	InitEGL();


	//GLTexture* renderTarget = new GLTexture(SCRWIDTH, SCRHEIGHT, GLTexture::INTTARGET);
	//#if WINBUILD
	//		Shader* shader = new Shader(
	//			"#version 330\nin vec4 p;\nin vec2 t;out vec2 u;void main(){u=t;gl_Position=p;}",
	//			"#version 330\nuniform sampler2D c;in vec2 u;out vec4 f;void main(){f=sqrt(texture(c,u));}", true);
	//#else
	//	Shader* shader = new Shader(
	//		"precision mediump float;attribute vec3 p;varying vec2 u;void main(){u=vec2(p.x*0.5+0.5,0.5-p.y*0.5);gl_Position=vec4(p,1);}",
	//		"precision mediump float;varying vec2 u;uniform sampler2D c;void main(){gl_FragColor=texture2D(c,u).zyxw;}",
	//		true);
	//#endif

	FixWorkingFolder();
	//Surface screen(SCRWIDTH, SCRHEIGHT);
	//screen.Clear(0);
	game = new Game();
	//game->SetTarget(&screen);
	game->Init();
	glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
	eglSwapInterval(eglDisplay, 0);

	/*pthread_t dummy;
	pthread_create(&dummy, 0, InputHandlerThread, 0);*/

	// Create ImGui context
	ImGui::CreateContext();

	// Set ImGui context as current
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 300 es");

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = static_cast<float>(SCRWIDTH); // Set to your actual width
	io.DisplaySize.y = static_cast<float>(SCRHEIGHT); // Set to your actual height
	while (!should_close)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProccessEvents(game);

		const float deltaTime = min(500.0f, 1000.0f * timer.elapsed());
		timer.reset();

		// start new ImGui frame for use inside game.tick()
		//imgui still throws erros when used with the current opengl setup
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
		io.DeltaTime = deltaTime / 1000.0f;

		game->Tick(deltaTime / 1000.0f);


		/*renderTarget->CopyFrom(&screen);
		shader->Bind();
		shader->SetInputTexture(0, "c", renderTarget);*/
		//DrawQuad();
		/*shader->Unbind();*/


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		eglSwapBuffers(eglDisplay, eglSurface);
		glFlush();
	}
}
