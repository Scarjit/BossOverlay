#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include <render/Device3D9.hpp>
#include <render/Surface3D9.hpp>

#include "BossTimer.h"
using namespace render;

//bool show_demo_window = true;
//bool show_another_window = false;
//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int windowstate = 0;
bool enabled = false;

void CustomImGUIStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding = ImVec2(14, 14);
	style.FramePadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(8, 8);
	style.ItemInnerSpacing = ImVec2(8, 8);

	ImVec4* colors = style.Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.98f, 0.39f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.88f, 0.41f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.47f, 0.98f, 0.26f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.98f, 0.61f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
}

void ChangeClickability(bool canclick, HWND ownd)
{
	long style = GetWindowLong(
		ownd, GWL_EXSTYLE);
	if (canclick) {
		style &= ~WS_EX_LAYERED;
		SetWindowLong(ownd,
			GWL_EXSTYLE, style);
		SetForegroundWindow(ownd);
		windowstate = 1;
	}
	else {
		style |= WS_EX_LAYERED;
		SetWindowLong(ownd,
			GWL_EXSTYLE, style);
		windowstate = 0;
	}
}



Device3D9::~Device3D9()
{
    /// Call to a virtual function inside a destructor will be statically resolved
    DTOR_EXECUTE_VIRTUAL( shutdown );
}

bool Device3D9::create( const std::string& target_window_title )
{
    if( !Overlay::create( target_window_title ) ) {
        return false;
    }
    if( FAILED( Direct3DCreate9Ex( D3D_SDK_VERSION, &m_Direct3D9Ex ) ) ) {
        return false;
    }
    
    D3DPRESENT_PARAMETERS params;
    ZeroMemory( &params, sizeof( D3DPRESENT_PARAMETERS ) );

    params.Windowed               = TRUE;
    params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    params.BackBufferFormat       = D3DFMT_A8R8G8B8;
    params.EnableAutoDepthStencil = TRUE;
    params.AutoDepthStencilFormat = D3DFMT_D16;
    params.MultiSampleType        = D3DMULTISAMPLE_NONE;
    params.PresentationInterval   = 0x80000000L;

    DWORD mulit_sample_quality = 0;
    if( SUCCEEDED( this->m_Direct3D9Ex->CheckDeviceMultiSampleType(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        D3DFMT_A8R8G8B8,
        true,
        D3DMULTISAMPLE_NONMASKABLE,
        &mulit_sample_quality ) ) ) {
        params.MultiSampleType    = D3DMULTISAMPLE_NONMASKABLE;
        params.MultiSampleQuality = mulit_sample_quality - 1;
    }
    if( FAILED( m_Direct3D9Ex->CreateDeviceEx(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        m_WndOverlay,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &params,
        nullptr,
        &m_Direct3D9Device ) ) ) {
        return false;
    }

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	CustomImGUIStyle();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_WndOverlay);
	ImGui_ImplDX9_Init(m_Direct3D9Device);

    if( FAILED( D3DXCreateSprite( m_Direct3D9Device, &m_Direct3DXSprite ) ) ) {
        return false;
    }

    m_Surface = std::make_unique<Surface3D9>( m_Direct3DXSprite );
    return m_Surface->initialize( m_Direct3D9Device );
}

bool click_enabled = false;

bool Device3D9::render()
{
    /// handle window messages
    if( !Overlay::render() ) {
        return false;
    }

    if( FAILED( m_Direct3D9Device->Clear(
        0,
        nullptr,
        D3DCLEAR_TARGET,
        0x00000000,
        1.f,
        0 )
    ) ) {
        return false;        
    }

    /// start the surface scene only when the target window
    /// is in foreground
    if( /*in_foreground()*/true ) {

		if (GetAsyncKeyState(VK_INSERT) & 1) enabled = !enabled;
		if (GetAsyncKeyState(VK_PAUSE) & 1) click_enabled = !click_enabled;

		if (windowstate != 1 && click_enabled) //You might need to remove this check and call it every time to get keyboard input working. Mouse input works anyway.
		{
			ChangeClickability(true, m_WndOverlay);
		}
    	
		if (enabled)
		{
			// Start the Dear ImGui frame
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();


			ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("L33T Hacks by S1mple", &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
			{
				
				if (ImGui::BeginTabBar("DummyTabBar", ImGuiTabBarFlags_None))
				{
					if (ImGui::BeginTabItem("Boss Timer"))
					{
						BossTimerTab();
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				
			}

			// Rendering
			ImGui::End();
		}
		else
		{
			if (windowstate != 0)
			{
				ChangeClickability(false, m_WndOverlay);
			}
		}

        /// if the surface has successfully started a new scene
        if( m_Surface->begin_scene() ) {
            ///-------------------------------------------------------------------------------------------------
            /// begin the rendering here
            ///-------------------------------------------------------------------------------------------------
			
			if (enabled)
			{
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			}

            execute_callbacks();
            ///-------------------------------------------------------------------------------------------------
            /// stop the rendering here
            ///-------------------------------------------------------------------------------------------------
            m_Surface->end_scene();
        }
    }

    m_Direct3D9Device->EndScene();
    m_Direct3D9Device->PresentEx(
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0
    );

    return true;
}

void Device3D9::shutdown()
{
    Overlay::shutdown();
    safe_release( &m_Direct3D9Ex );
    safe_release( &m_Direct3D9Device );
    safe_release( &m_Direct3DXSprite );

    if( m_Surface ) {
        m_Surface->shutdown();
    }
}