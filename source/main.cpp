#include <iostream>
#include <memory>

#include "SDL.h"
#include "SDL_opengl.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"
#include "ImGuiFileDialog.h"

#include "Clock.hpp"
#include "ImageSourceNode.hpp"
#include "NodeBase.hpp"
#include "NodeEditor.hpp"
#include "VideoSourceNode.hpp"

int main(int argc, char **argv)
{
    // Initialize SDL system
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    const char* glsl_version = "#version 130";

    // Create window
    int32_t win_width  = 1280;
    int32_t win_height = 720;
    SDL_WindowFlags win_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Window *win = SDL_CreateWindow("NodeImageProcessing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, win_flags);
    //SDL_Window *win = SDL_CreateWindow("NodeImageProcessing", 100, 1500, win_width, win_height, win_flags);

    SDL_GLContext gl_context = SDL_GL_CreateContext(win);
    SDL_GL_MakeCurrent(win, gl_context);
    SDL_GL_SetSwapInterval(1);

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGuiContext *imgui_context = ImGui::CreateContext();
    ImNodesContext *imnodes_context = ImNodes::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(win, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Initialize Node editor
    NodeEditor::get().init();

    // Initialize file dialog settings
    ImGuiFileDialog::Instance()->SetCreateThumbnailCallback([](IGFD_Thumbnail_Info *vThumbnail_Info) -> void
    {
        if (vThumbnail_Info && 
            vThumbnail_Info->isReadyToUpload && 
            vThumbnail_Info->textureFileDatas)
        {
            GLuint textureId = 0;
            glGenTextures(1, &textureId);
            vThumbnail_Info->textureID = (void*)(intptr_t)textureId;

            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                (GLsizei)vThumbnail_Info->textureWidth, (GLsizei)vThumbnail_Info->textureHeight, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, vThumbnail_Info->textureFileDatas);
            glFinish();
            glBindTexture(GL_TEXTURE_2D, 0);

            delete[] vThumbnail_Info->textureFileDatas;
            vThumbnail_Info->textureFileDatas = nullptr;

            vThumbnail_Info->isReadyToUpload = false;
            vThumbnail_Info->isReadyToDisplay = true;
        }
    });

    ImGuiFileDialog::Instance()->SetDestroyThumbnailCallback([](IGFD_Thumbnail_Info* vThumbnail_Info)
    {
        if (vThumbnail_Info)
        {
            GLuint texID = (GLuint)(intptr_t)vThumbnail_Info->textureID;
            glDeleteTextures(1, &texID);
            glFinish();
        }
    });

    // Font settings
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Build();

    // Set style
    ImGui::StyleColorsDark();
    ImVec4 bg_color = ImVec4(0.4f, 0.4f, 0.4f, 1.f);

    // Debug nodes
    //NodeEditor::get().newVideoNode(NT_VideoSource, "sample/a.mp4");

    bool done = false;
    while (!done)
    {
        // Process terminate input
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            ImGui_ImplSDL2_ProcessEvent(&ev);

            if (ev.type == SDL_QUIT)
            {
                done = true;
            }
            else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE && ev.window.windowID == SDL_GetWindowID(win))
            {
                done = true;
            }
            else if (ev.key.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.scancode == SDL_SCANCODE_BACKSPACE || ev.key.keysym.scancode == SDL_SCANCODE_DELETE)
                {
                    const uint32_t selected_node_num = ImNodes::NumSelectedNodes();
                    if (selected_node_num > 0)
                    {
                        // Get selected nodes
                        std::vector<int32_t> selected_node_id;
                        selected_node_id.resize(selected_node_num);
                        ImNodes::GetSelectedNodes(selected_node_id.data());

                        for (int32_t id : selected_node_id)
                        {
                            auto node_iter = std::find_if(
                                NodeEditor::get().node_list_.begin(),
                                NodeEditor::get().node_list_.end(),
                                [id](NodeBase* node)
                                {
                                    return id == node->id_;
                                }
                            );

                            NodeEditor::get().deleteNode(*node_iter);
                        }
                    }
                }
            }
        }

        // Update main window size
        SDL_GetWindowSize(win, &win_width, &win_height);

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Begin drawing window
        ImGui::SetNextWindowSize(ImVec2(win_width, win_height), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));

        ImGui::Begin("NodeImageProcessing", nullptr, ImGuiWindowFlags_MenuBar);

        /////////////////////////
        ///// Draw menu bar /////
        /////////////////////////
        // File menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Open dialog to select image file
                if (ImGui::MenuItem("Open Image File"))
                {
                    const char* filters = "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}";
                    ImGuiFileDialog::Instance()->OpenDialog("SelectImageDlgKey", "Select Image File", filters, ".");
                }

                // Open dialog to select video file
                if (ImGui::MenuItem("Open Video File"))
                {
                    const char* filters = "Video files (*.mp4){.mp4}";
                    ImGuiFileDialog::Instance()->OpenDialog("SelectVideoDlgKey", "Select Video File", filters, ".");
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // Exit button
                if (ImGui::MenuItem("Exit"))
                {
                    done = true;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Edit menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Image/Video"))
            {
                // Create new ImageNode
                if (ImGui::MenuItem("ImageSource"))
                {
                    NodeEditor::get().newImageNode(NT_ImageSource);
                }

                // Create new VideoNode
                if (ImGui::MenuItem("VideoSource"))
                {
                    NodeEditor::get().newVideoNode(NT_VideoSource);
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // Create new BinariztionNode
                if (ImGui::MenuItem("Binarization"))
                {
                    NodeEditor::get().newImageNode(NT_Binarization);
                }

                // Create new EdgeDetectionNode
                if (ImGui::MenuItem("EdgeDetection"))
                {
                    NodeEditor::get().newImageNode(NT_EdgeDetection);
                }

                // Create new GrayScalingNode
                if (ImGui::MenuItem("GrayScaling"))
                {
                    NodeEditor::get().newImageNode(NT_GrayScaling);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Others
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Others"))
            {
                // Create new InformationNode
                if (ImGui::MenuItem("Information"))
                {
                    NodeEditor::get().newImageNode(NT_InformationNode);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        //////////////////////////////////////
        ///// Draw dialog to select file /////
        //////////////////////////////////////
        // Draw dialog to select image file
        ImGuiFileDialog::Instance()->ManageGPUThumbnails();

        ImVec2 dialog_max_size = ImVec2((float)win_width, (float)win_height);
        ImVec2 dialog_min_size = ImVec2((float)win_width * 0.5f, (float)win_height * 0.5f);

        if (ImGuiFileDialog::Instance()->Display("SelectVideoDlgKey", ImGuiWindowFlags_NoCollapse, dialog_min_size, dialog_max_size))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string file_path = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string current_path = ImGuiFileDialog::Instance()->GetCurrentPath();

                if (dynamic_cast<VideoSourceNode*>(NodeEditor::get().tmp_node_))
                    ((VideoSourceNode*)NodeEditor::get().tmp_node_)->loadSource(file_path.c_str());
                else
                    NodeEditor::get().newVideoNode(NT_VideoSource, file_path.c_str());
            }
            
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display("SelectImageDlgKey", ImGuiWindowFlags_NoCollapse, dialog_min_size, dialog_max_size))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string file_path = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string current_path = ImGuiFileDialog::Instance()->GetCurrentPath();

                if (dynamic_cast<ImageSourceNode*>(NodeEditor::get().tmp_node_))
                    ((ImageSourceNode*)NodeEditor::get().tmp_node_)->loadSource(file_path.c_str());
                else
                    NodeEditor::get().newImageNode(NT_ImageSource, file_path.c_str());
            }
            
            ImGuiFileDialog::Instance()->Close();
        }

        //////////////////////
        ///// Draw nodes /////
        //////////////////////
        NodeEditor::get().draw();

        // End drawing window
        ImGui::End();

        /////////////////////
        ///// Rendering /////
        /////////////////////
        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(
            bg_color.x * bg_color.w,
            bg_color.y * bg_color.w,
            bg_color.z * bg_color.w,
            bg_color.w
        );
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(win);
    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    // Clean up ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    ImNodes::DestroyContext();
    ImGui::DestroyContext();

    // Quit SDL system
    SDL_Quit();
}
