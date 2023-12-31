#include "ServiceRunner.h"
#include "StrConvert.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <map>

#include <GLFW/glfw3.h>

static void getDesktopResolution(int *horizontal, int *vertical)
{
   RECT desktop;

   const HWND hDesktop = GetDesktopWindow();

   GetWindowRect(hDesktop, &desktop);

   *horizontal = desktop.right;
   *vertical = desktop.bottom;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nShowCmd);

	// Setup window
    glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}

	// Config Window GLFW
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // BorderLess
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GL 3.0 + GLSL 330
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Width & Height Window
	static const int windowSizeWidth = 640;
	static const int windowSizeHeight = 480;

	// Get Desktop Resolution
	static int horizontal = 0;
    static int vertical = 0;
    getDesktopResolution(&horizontal, &vertical);

    // Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(windowSizeWidth, windowSizeHeight, "Dear ImGui Service Runner", NULL, NULL);
	if (window == NULL) {
		return EXIT_FAILURE;
	}

	// Make the window pos centered
	glfwSetWindowPos(window, (horizontal / 2) - (windowSizeWidth / 2), (vertical / 2) - (windowSizeHeight / 2));

	// Hide Command Prompt Executable if entry point is main, if entry point is WinMain just comment or delete
	// ShowWindow(GetConsoleWindow(), SW_HIDE);
    
	// Create Context OpenGL
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui Style
    ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 8.0f;

    // Setup Platform/Renderer Backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
	
    // Our State
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// fstream
	static std::ofstream ofile("Service.dat", std::ios::app);
	static std::ifstream ifile("Service.dat");
	ofile.close();

	// Window ImGui
	static bool isOpenMainWindow = true;

	// Add Service Vars
	static char inputServiceName[128] = "";

	// Vector svcnames
	static std::vector<std::string> svcnames;

	// fstream Logic Read File
	static std::string svcNameBuffer;
	while(std::getline(ifile, svcNameBuffer)) {
		svcnames.push_back(svcNameBuffer);
	}

	// Initialize bool variables for each service
	std::map<std::string, bool> svcCheckBox;
	std::map<std::string, DWORD> svcErrorCode;
	std::map<std::string, bool> svcStatus;

	std::map<std::string, bool> selectionStatus;

	for (const auto& svcName : svcnames) {
		svcCheckBox[svcName] = false;
		svcErrorCode[svcName] = NO_ERROR;
		svcStatus[svcName] = false;
    }
	ifile.close();

	for (const auto& svcName : svcnames) {
		ServiceRunner::svcCheckStatus(StrConvert::ConvertToWideString(svcName), &svcErrorCode[svcName], &svcCheckBox[svcName]);
	}

	// Update
	static bool updateService = false;

	// Boolean Vars
	static bool isDuplicate = false;
	static bool isSuccessAddSvc = false;
	static bool isEmptyInput = false;



    // Main loop
	while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		// Close Window
		if (isOpenMainWindow == false) {
			break; // Out from Main Loop
		}

		// Our ImGui Code
		ImGui::SetNextWindowSize(ImVec2((float)windowSizeWidth, (float)windowSizeHeight));
		ImGui::SetWindowPos("Service Runner", ImVec2(0, 0));
		if (ImGui::Begin("Service Runner", &isOpenMainWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {

			// Make a TabBar
			if(ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {

				// TabBar Service
				if(ImGui::BeginTabItem("Service")) {
					ImGui::Text("Make sure your run this program as Administrator.");
					ImGui::Separator();
					ImGui::Spacing();
					
					for (const auto& svcName : svcnames) {
						if (ServiceRunner::svcCheckRegistered(StrConvert::ConvertToWideString(svcName))) {
							if (ImGui::Checkbox(svcName.c_str(), &svcCheckBox[svcName])) {
								if (svcStatus[svcName] == false) {
									ServiceRunner::svcWinApiStart(StrConvert::ConvertToWideString(svcName), &svcErrorCode[svcName], &svcStatus[svcName]);
								}
							} else {
								if (svcStatus[svcName] == true && svcCheckBox[svcName] == false) {
									ServiceRunner::svcWinApiStop(StrConvert::ConvertToWideString(svcName), &svcErrorCode[svcName], &svcStatus[svcName]);
								}
							}
						} else {
							if (ImGui::Checkbox((svcName + " [ Not Registered ]").c_str(), &svcCheckBox[svcName])) {
								if (svcStatus[svcName] == false) {
									ServiceRunner::svcWinApiStart(StrConvert::ConvertToWideString(svcName), &svcErrorCode[svcName], &svcStatus[svcName]);
								}
							} else {
								if (svcStatus[svcName] == true && svcCheckBox[svcName] == false) {
									ServiceRunner::svcWinApiStop(StrConvert::ConvertToWideString(svcName), &svcErrorCode[svcName], &svcStatus[svcName]);
								}
							}
						}
					}

					ImGui::EndTabItem();
				}

				// TabBar Input
				if(ImGui::BeginTabItem("Input")) {
					ImGui::Text("Make sure the service is registered locally.");
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Text("Add Service :");
					ImGui::Spacing();
					ImGui::InputTextWithHint("##inputservice", "Service Name", inputServiceName, IM_ARRAYSIZE(inputServiceName), ImGuiInputTextFlags_CharsNoBlank);
					ImGui::Spacing();

					if (ImGui::Button("Submit")) {
						bool isCanWrite = false;
						isEmptyInput = false;

						// Validation Null Input
						if (strcmp(inputServiceName, "") == 0) {
							isEmptyInput = true;
							isDuplicate = false;
							isCanWrite = false;
							updateService = false;
						} else {
							if (svcnames.size() < 1) {
								isDuplicate = false;
								isCanWrite = true;
							} else {
								for (const auto& svcName : svcnames) {
									if (StrConvert::toLowcase(inputServiceName) == StrConvert::toLowcase(svcName)) {
										isDuplicate = true;
										isCanWrite = false;
										break;
									}

									isDuplicate = false;
									isCanWrite = true;
								}
							}
						}

						// Write to file Service.dat
						if (isCanWrite) {	
							ofile.open("Service.dat", std::ios::app);
							ofile << inputServiceName << std::endl;
							ofile.close();
							updateService = true;
							isSuccessAddSvc = true;
						}

						// Update Service
						if (updateService) {
							ifile.open("Service.dat");
							static std::string bufferLastLine;

							while(std::getline(ifile, svcNameBuffer)) {
								bufferLastLine = svcNameBuffer;
							}

							svcnames.push_back(bufferLastLine);
							svcCheckBox.insert(std::make_pair(svcnames.back(), false));
							svcErrorCode.insert(std::make_pair(svcnames.back(), NO_ERROR));
							svcStatus.insert(std::make_pair(svcnames.back(), false));

							ifile.close();
							updateService = false;
						}
					}

					if (isEmptyInput) {
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Text("Error : Please avoid to input empty string.");
					} else if (isDuplicate) {
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Text("Error : Service has been registered, please avoid duplicating input.");
					} else if (isSuccessAddSvc) {
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Text("Message : Success add Service.");
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		} ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	// Close window GLFW and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

	return EXIT_SUCCESS;
}