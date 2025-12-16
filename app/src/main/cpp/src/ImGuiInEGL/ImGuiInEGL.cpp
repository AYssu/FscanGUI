#include "ImGuiInEGL.h"

JNIEnv *env;
jclass cls;
jmethodID mid;


ImGuiInEGL::ImGuiInEGL()
{
    EGLConfig = nullptr;

    EglDisplay = EGL_NO_DISPLAY;
    EglSurface = EGL_NO_SURFACE;
    EglContext = EGL_NO_CONTEXT;
}

int ImGuiInEGL::InItEGL()
{
    EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EglDisplay == EGL_NO_DISPLAY)
    {
        return -1;
    }
    EGLint *version = new EGLint[2];
    if (!eglInitialize(EglDisplay, &version[0], &version[1]))
    {
        return -1;
    }
    const EGLint attribs[] = {EGL_BUFFER_SIZE, 32, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
                              EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_DEPTH_SIZE, 8,
                              EGL_STENCIL_SIZE, 8,
                              EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, EGL_SURFACE_TYPE,
                              EGL_WINDOW_BIT, EGL_NONE};
    EGLint num_config;
    if (!eglGetConfigs(EglDisplay, NULL, 1, &num_config))
    {
        return -1;
    }
    if (!eglChooseConfig(EglDisplay, attribs, &EGLConfig, 1, &num_config))
    {
        return -1;
    }
    int attrib_list[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    EglContext = eglCreateContext(EglDisplay, EGLConfig, EGL_NO_CONTEXT, attrib_list);
    if (EglContext == EGL_NO_CONTEXT)
    {
        return -1;
    }
    EglSurface = eglCreateWindowSurface(EglDisplay, EGLConfig, SurfaceWin, NULL);
    if (EglSurface == EGL_NO_SURFACE)
    {
        return -1;
    }
    if (!eglMakeCurrent(EglDisplay, EglSurface, EglSurface, EglContext))
    {
        return -1;
    }
    return 1;
}

int ImGuiInEGL::SwapBuffers()
{
    if (eglSwapBuffers(EglDisplay, EglSurface))
    {
        return 1;
    }
    return 0;
}

void ImGuiInEGL::CleaBuffers()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFlush();
}

void ImGuiInEGL::ImGuiMainWinStart()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
}

void ImGuiInEGL::ImGuiMainWinEnded()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiInEGL::SetInPut(ImGuiInPut *InPut_)
{
    this->InPut = InPut_;
}



int ImGuiInEGL::InItImGui()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    Io = &ImGui::GetIO();
    Io->ConfigWindowsMoveFromTitleBarOnly = false;
    Style = &ImGui::GetStyle();
    Style->WindowTitleAlign = ImVec2(0.5, 0.5);
    Style->TabRounding = 10.0f;
    Style->FramePadding = ImVec2(0, 10);


    Style->ScrollbarSize = 40.0f;
    Style->ScrollbarRounding = 10.f;
    Style->GrabRounding = 10.0f;
    Style->GrabMinSize = 30.0f;
    Style->FrameBorderSize = 0.f;
    Style->FrameRounding = 20.0f;
    Style->WindowRounding = 10.f;
    Context = ImGui::GetCurrentContext();
    ImGui_ImplAndroid_Init(this->SurfaceWin);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImFontConfig fontConfig;

    default_random_engine e;


    fontConfig.FontDataOwnedByAtlas = false;
    static const ImWchar icons_ranges[] = {0xf000, 0xf3ff, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;
    IM_ASSERT(Io->Fonts != NULL);
    ImGui::StyleColorsClassic();
    return 1;
}

void
ImGuiInEGL::onSurfaceCreate(JNIEnv *env, jobject Surface, int Width, int Height)
{
    this->SurfaceWin = ANativeWindow_fromSurface(env, Surface);
    this->Width = Width;
    this->Height = Height;
    // 确保首帧就使用有效视口
    this->isChange = true;
    std::thread SurfaceThread([this]{ EGLThread(); });
    SurfaceThread.detach();
}

void ImGuiInEGL::onSurfaceDestroy()
{
}

void ImGuiInEGL::onSurfaceChange(int Width, int Heigth)
{
    this->Width = Width;
    this->Height = Heigth;
    this->isChange = true;
}

void __attribute__((annotate("sub fla split"))) ImGuiInEGL::EGLThread()
{
    this->InItEGL();
    this->InItImGui();

    InPut->SetImGuiIo(Io);

    while (true)
    {
        if (this->isChange)
        {
            glViewport(0, 0, this->Width, this->Height);
            this->isChange = false;
        }

        this->CleaBuffers();

        ImGuiMainWinStart();

        // 设定窗口初始位置/尺寸（可拖动、可自动适配内容）
        ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("ImGuiMainWin", NULL,
                     ImGuiWindowFlags_NoCollapse);
        // 记录窗口指针，供宽高查询
        Window = ImGui::GetCurrentWindow();
        // 一个可拖动的简单菜单和按钮
        ImGui::TextUnformatted("ImGui overlay running");
        if (ImGui::Button("Click Me")) {
            // 点击后可在此触发事件或调用 native/Java 逻辑
        }
        ImGui::End();
        ImGuiMainWinEnded();
        this->SwapBuffers();

    }
    glViewport(0, 0, 0, 0);
    this->CleaBuffers();
    this->SwapBuffers();
}

