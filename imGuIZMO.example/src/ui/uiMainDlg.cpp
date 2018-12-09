///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2018 Michele Morrone
//  All rights reserved.
//
//  mailto:me@michelemorrone.eu
//  mailto:brutpitt@gmail.com
//  
//  https://github.com/BrutPitt
//
//  https://michelemorrone.eu
//  
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the <organization> nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//  
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

void setRotation(const glm::quat &q);
glm::quat& getRotation();


#include "../qJulia.h"
#include "../glApp.h"
#include "../glWindow.h"


#include "uiMainDlg.h"
#include "../tools/imGuIZMO.h"
using namespace glm;


    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



void mainImGuiDlgClass::renderImGui()
{

    static bool metricW = false, demosW = false;
    if(!visible()) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiStyle& style = ImGui::GetStyle();
           


   ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);
   ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_FirstUseEver);


    bool isVisible = true;
        
    if(ImGui::Begin("qaternion Julia set", &isVisible)) {
        ImGui::BeginGroup(); {

            const float w = ImGui::GetContentRegionAvailWidth();
            const float half = w/2.f;
            const float third = w/3.f;



            ImGui::Text(" qJulia coordinates qX/qY/qZ/qW");
            ImGui::PushItemWidth(w);
            ImGui::DragFloat4("##qwx", glm::value_ptr(theWnd->qjSet->quatPt),.001, -1.0, 1.0);
            ImGui::PopItemWidth();
            ImGui::Text(" ");
            ImGui::Text("rendering components:");
            ImGui::PushItemWidth(half);

            ImGui::Text(" Specular Exp");
            ImGui::SameLine(half);
            ImGui::Text(" Specular Comp");
            

            ImGui::DragFloat("##Specular Exp", &theWnd->qjSet->specularExponent,.1, 1.0, 250.0,"%.3f",2.f);
            ImGui::SameLine(half);
            ImGui::DragFloat("##Specular Comp", &theWnd->qjSet->specularComponent,.001, 0.0, 1.0);

            ImGui::Text(" Normal ColorInt.");
            ImGui::SameLine(half);
            ImGui::Text(" Accuracy");

            ImGui::DragFloat("##Normal Color", &theWnd->qjSet->normalComponent,.001, 0.0, 1.0);
            ImGui::SameLine(half);
            float f = theWnd->qjSet->epsilon * 1000.0;
            if(ImGui::SliderFloat("##Accuracy", &f,.001, 1.0,"%.6f",5.f)) theWnd->qjSet->epsilon = f/1000.0;
            ImGui::PopItemWidth();

            ImGui::PushItemWidth(third);
            ImGui::Checkbox("Shadow", &theWnd->qjSet->useShadow); 
            ImGui::SameLine(third);
            ImGui::Checkbox("Occlusion", &theWnd->qjSet->useAO);
            ImGui::SameLine(third*2.f);
            ImGui::Checkbox("full render", &theWnd->qjSet->isFullRender);
            ImGui::PopItemWidth();

            ImGui::Text(" ");
            ImGui::Text("Light & colors");

            ImGui::Text(" Diffuse Color");
            ImGui::PushItemWidth(w);
            ImGui::ColorEdit3("##diffuse",glm::value_ptr(theWnd->qjSet->diffuseColor));
            ImGui::PopItemWidth();

            glm::vec3 vL(-theWnd->qjSet->Light);
            if(ImGui::gizmo3D("Light dir", vL) )  theWnd->qjSet->Light = -vL;

            ImGui::PushItemWidth(half);    
            if(ImGui::Button("show metrics",ImVec2(half, 0))) metricW ^=1;   ImGui::SameLine();
            if(ImGui::Button("ImGui Demo", ImVec2(half, 0))) demosW ^=1;
            ImGui::PopItemWidth();


            
            } ImGui::EndGroup(); 

        } ImGui::End();

    static bool vis = true;
    //quaternionf qt = theWnd->getTrackball().getRotation();
    glm::quat qt = getRotation();

    float sz=240;
    ImGui::SetNextWindowSize(ImVec2(sz, theApp->GetHeight()), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(theApp->GetWidth()-sz,0 ), ImGuiCond_Always);
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0.f,0.f,0.f,0.f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg,ImVec4(0.f,0.f,0.f,0.f));
    
    const float prevWindowBorderSize = style.WindowBorderSize;
    style.WindowBorderSize = .0f;

    ImGui::Begin("##giz", &vis, ImGuiWindowFlags_NoTitleBar|
                                ImGuiWindowFlags_NoResize|
                                //ImGuiWindowFlags_NoInputs|
                                ImGuiWindowFlags_NoScrollbar);

    

    
    ImGui::SetCursorPos(ImVec2(0,0));
    ImGui::PushItemWidth(sz*.25-2);
    ImVec4 oldTex(style.Colors[ImGuiCol_Text]);
    bool quatChanged=false;
    style.Colors[ImGuiCol_Text].x = 1.0, style.Colors[ImGuiCol_Text].y = style.Colors[ImGuiCol_Text].z =0.f;
    if(ImGui::DragFloat("##u0",(float *)&qt.x,0.01f, -1.0, 1.0, "x: %.2f",1.f)) quatChanged=true; ImGui::SameLine();
    style.Colors[ImGuiCol_Text].y = 1.0, style.Colors[ImGuiCol_Text].x = style.Colors[ImGuiCol_Text].z =0.f;
    if(ImGui::DragFloat("##u1",(float *)&qt.y,0.01f, -1.0, 1.0, "y: %.2f",1.f)) quatChanged=true;  ImGui::SameLine();
    style.Colors[ImGuiCol_Text].z = 1.0, style.Colors[ImGuiCol_Text].y = style.Colors[ImGuiCol_Text].x =0.f;
    if(ImGui::DragFloat("##u2",(float *)&qt.z,0.01f, -1.0, 1.0, "z: %.2f",1.f)) quatChanged=true;  ImGui::SameLine();
    style.Colors[ImGuiCol_Text] = oldTex;
    if(ImGui::DragFloat("##u3",(float *)&qt.w,0.01f, -1.0, 1.0, "w: %.2f",1.f)) quatChanged=true;
    ImGui::PopItemWidth();

    //If you modify quaternion parameters outside control, with DragFloat or other, remembre to NORMALIZE it
    if(quatChanged) setRotation(glm::normalize(qt));
    ImGui::DragFloat3("Light",glm::value_ptr(theWnd->qjSet->Light),0.01f);

    glm::vec3 lL(-theWnd->qjSet->Light);
    if(ImGui::gizmo3D("##aaa", qt, lL, sz))  { 
        theWnd->qjSet->Light = -lL;
        setRotation(qt);
    }

    sz*=.5;
    static glm::quat qt2(1.0f,0,0,0);
    static glm::quat qt3(1.0f,0,0,0);
    static glm::quat qt4(1.0f,0,0,0);

    static glm::vec3 a(1.f);
    static glm::vec4 b(1.0,0.0,0.0,0.0);
    static glm::vec4 c(1.0,0.0,0.0,0.0);
    static glm::vec4 d(1.0,0.0,0.0,0.0);

    static float resSolid = 1.0;
    static float axesLen = .95;
    static float axesThickness = 1.0;
    glm::vec3 resAxes(axesLen,axesThickness,axesThickness);
    static glm::vec3 dirCol(1.0,.5,0.0);
    static ImVec4 sphCol1(ImGui::ColorConvertU32ToFloat4(0xff0080ff));
    static ImVec4 sphCol2(ImGui::ColorConvertU32ToFloat4(0xffff8000));

    //ImGui::PushItemWidth(sz);
    //quat form gizmo is normalized!
    char s[50];
    if(ImGui::gizmo3D("##gizmo1", qt, sz)) {  setRotation(qt); }

    ImGui::SameLine();

    glm::vec3 vL(-theWnd->qjSet->Light);
    if(ImGui::gizmo3D("##Dir1", vL,sz, imguiGizmo::sphereAtOrigin) )  theWnd->qjSet->Light = -vL;

    static bool otherExamples = false;

    ImVec2 cPos(ImGui::GetCursorPos());

    ImGui::SetCursorPos(ImVec2(0,theApp->GetHeight()-ImGui::GetFrameHeightWithSpacing()*7));

    if(!otherExamples) {
        ImGui::Text("imguiGizmo usage:");
        ImGui::Text("- Left/Right mouse buttons:");
        ImGui::Text("      FREE move axes/spot");
        ImGui::Text("- Shft+btn -> rot ONLY around X");
        ImGui::Text("- Ctrl+btn -> rot ONLY around Y");
        ImGui::Text("- Alt|Super+btn-> rot ONLY on Z");
        ImGui::Text("- Both btns to move together...");
    }

    ImGui::SetCursorPos(ImVec2(0,theApp->GetHeight()-ImGui::GetFrameHeightWithSpacing()));
    if(ImGui::Button(" -= Show more examples =- ")) otherExamples ^=1;

    if(otherExamples) {

        ImGui::SetCursorPos(cPos);
        imguiGizmo::resizeAxesOf(resAxes);
        imguiGizmo::resizeSolidOf(resSolid); // sphere bigger
        if(ImGui::gizmo3D("##RotB", b,sz, imguiGizmo::sphereAtOrigin))  {
            setRotation(glm::angleAxis(b.w, vec3(b)));   
        }   //
        imguiGizmo::restoreSolidSize(); // restore at default
        imguiGizmo::restoreAxesSize();

        ImGui::SameLine();

        imguiGizmo::resizeSolidOf(.75); // sphere bigger
        if(ImGui::gizmo3D("##RotB1", qt3, d,sz, imguiGizmo::sphereAtOrigin))  {   } 
        imguiGizmo::restoreSolidSize(); // restore at default


        imguiGizmo::resizeAxesOf(glm::vec3(imguiGizmo::axesResizeFactor.x, 1.75, 1.75));
        imguiGizmo::resizeSolidOf(1.5); // sphere bigger
        imguiGizmo::setSphereColors(ImGui::ColorConvertFloat4ToU32(sphCol1), ImGui::ColorConvertFloat4ToU32(sphCol2));
        //c = glm::vec4(glm::axis(qt), glm::angle(qt)); 
        if(ImGui::gizmo3D("##RotC", c,sz, imguiGizmo::sphereAtOrigin|imguiGizmo::modeFullAxes)) {}   //theWnd->getTrackball().setRotation(glm::angleAxis(c.w, vec3(c)));   
        imguiGizmo::restoreSolidSize(); // restore at default
        imguiGizmo::restoreSphereColors();
        imguiGizmo::restoreAxesSize();

        ImGui::SameLine();


        //imguiGizmo::resizeAxesOf(glm::vec3(2.5, 2.5, 2.5));
        imguiGizmo::resizeAxesOf(resAxes);
        imguiGizmo::resizeSolidOf(resSolid); // sphere bigger
        if(ImGui::gizmo3D("##tZ", qt2, qt4, sz, imguiGizmo::modeFullAxes)) { setRotation(qt); }
        imguiGizmo::restoreSolidSize(); // restore at default
        imguiGizmo::restoreAxesSize();
        //imguiGizmo::restoreAxesSize();
    
        imguiGizmo::resizeAxesOf(resAxes);
        imguiGizmo::resizeSolidOf(resSolid*2); // sphere bigger
        if(ImGui::gizmo3D("##tZ2", qt2, sz, imguiGizmo::cubeAtOrigin|imguiGizmo::modeFullAxes)) { setRotation(qt); }
        imguiGizmo::restoreSolidSize(); // restore at default
        imguiGizmo::restoreAxesSize();

        ImGui::SameLine();   

        imguiGizmo::resizeAxesOf(resAxes);
        //this is only direction!!!... and i can change color
        imguiGizmo::setDirectionColor(ImVec4(dirCol.x,dirCol.y, dirCol.z, 1.0));
        if( ImGui::gizmo3D("##RotA", a,sz,0)) {}   
        imguiGizmo::restoreDirectionColor();
        imguiGizmo::restoreAxesSize();

    }


    
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    style.WindowBorderSize = prevWindowBorderSize;

    
    const int dimY =260;
    ImGui::SetNextWindowSize(ImVec2(500, dimY), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(0,theWnd->GetHeight()-dimY), ImGuiCond_FirstUseEver);

    static bool vertexChange = false;

    if(ImGui::Begin("gizmo options", &isVisible, ImGuiWindowFlags_NoScrollbar)) {
        ImGui::BeginGroup(); {
            ImGui::Columns(2);
            const float w = ImGui::GetContentRegionAvailWidth();
            const float half = w/2.f;
            const float third = w/3.f;

            static int mode_idx = 0;
            static int mode = imguiGizmo::mode3Axes;

            static int draw_idx = 0;
            static int draw = imguiGizmo::cubeAtOrigin;

            if (ImGui::Combo("Modes##combo", &mode_idx, "Axes (default)\0"\
                                                              "Direction\0"\
                                                              "Dual mode\0"\
                            )) 
            {
                switch (mode_idx)
                {
                    case 0: mode = imguiGizmo::mode3Axes; break;
                    case 1: mode = imguiGizmo::modeDirection; break;
                    case 2: mode = imguiGizmo::modeDual;  break;

                }
            }


            if (ImGui::Combo("Apparence##combo", &draw_idx,  "Cube in center (default)\0"\
                                                              "Sphere in center\0"\
                                                              "no solids in center\0"\
                            )) 
            {
                switch (draw_idx)
                {
                    case 0: draw = imguiGizmo::cubeAtOrigin; break;
                    case 1: draw = imguiGizmo::sphereAtOrigin; break;
                    case 2: draw = imguiGizmo::noSolidAtOrigin;  break;
                }
            }
            static bool isFull;
            ImGui::Checkbox("Show full axes (default false)", &isFull); 

            ImGui::Text(" ");
            ImGui::Text(" Axes/Arrow/Solids resize");
            ImGui::PushItemWidth(third);    
            ImGui::DragFloat("##res1",&axesLen ,0.01f, 0.0, 1.0, "len %.2f");
            ImGui::SameLine();
            ImGui::DragFloat("##res2",&axesThickness ,0.01f, 0.0, 8.0, "thick %.2f");
            ImGui::SameLine();
            ImGui::DragFloat("##res3",&resSolid ,0.01f, 0.0, 8.0, "solids %.2f");
            ImGui::PopItemWidth();

            if(!(mode & imguiGizmo::mode3Axes) ) {
                ImGui::Text(" Color singleDirection");
                ImGui::ColorEdit3("##Direction",glm::value_ptr(dirCol));
            }
    
            if((draw & imguiGizmo::sphereAtOrigin) && !(mode & imguiGizmo::modeDirection)) {
                ImGui::Text(" Color Sphere");
                ImGui::PushItemWidth(half);    
                ImGui::ColorEdit3("##Sph1",(float *) &sphCol1);
                //ImGui::SameLine();
                ImGui::ColorEdit3("##Sph2",(float *) &sphCol2);
                ImGui::PopItemWidth();
            }



            if(isFull) draw |= imguiGizmo::modeFullAxes;
            else       draw &= ~imguiGizmo::modeFullAxes;

            static glm::quat qv1(1.0f,0,0,0);
            static glm::quat qv2(1.0f,0,0,0);

            ImGui::SetCursorPos(ImVec2(0,dimY-ImGui::GetFrameHeightWithSpacing()));

            if(ImGui::Button(" -= Change solids attributes =- ")) vertexChange ^=1;


            ImGui::NextColumn();
            imguiGizmo::resizeAxesOf(resAxes);
            imguiGizmo::resizeSolidOf(resSolid); // sphere bigger
            imguiGizmo::setSphereColors(ImGui::ColorConvertFloat4ToU32(sphCol1), ImGui::ColorConvertFloat4ToU32(sphCol2));
            imguiGizmo::setDirectionColor(ImVec4(dirCol.x,dirCol.y, dirCol.z, 1.0));


            if(mode != imguiGizmo::modeDual) ImGui::gizmo3D("##gizmoV1", qv1, w, mode|draw );
            else                           ImGui::gizmo3D("##gizmoV2", qv1, qv2, w, mode|draw);

            imguiGizmo::restoreSolidSize();
            imguiGizmo::restoreDirectionColor();
            imguiGizmo::restoreAxesSize();
            imguiGizmo::restoreSphereColors();


            } ImGui::EndGroup();


        } ImGui::End();


    const int dimYz =200;
    ImGui::SetNextWindowSize(ImVec2(450, dimYz), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(250,0), ImGuiCond_FirstUseEver);
    if(vertexChange) {
        if(ImGui::Begin("Vertex rebuild", &vertexChange)) {
            ImGui::BeginGroup(); {
                ImGui::Columns(2);
                ImGui::TextWrapped("All vertexes of all solids (cube, cone, cyl, sphere) are processed only once on startup\
 and are invariant for all widgets in application.\n\
Although the proportion can be modified for each individual control (as already seen), the ratio and # faces are fixed.\n\
Anyhow the static variables can be modified to change the 3d aspect of all solids, and rebuild they... una tantum");

                ImGui::NextColumn();
                static bool needRebuild=false;
                if(ImGui::SliderInt("Cone Slices", &imguiGizmo::coneSlices, 3, 30)) needRebuild=true;
                if(ImGui::SliderFloat("Cone Len", &imguiGizmo::coneLength, 0.01, .5))  needRebuild=true;
                if(ImGui::SliderFloat("Cone Radius", &imguiGizmo::coneRadius, 0.01, .3))  needRebuild=true;

                if(ImGui::SliderInt("Cyl Slices", &imguiGizmo::cylSlices, 3, 30))  needRebuild=true;
                if(ImGui::SliderFloat("Cyl Radius", &imguiGizmo::cylRadius, 0.001, .5))  needRebuild=true;

                static int sphTess_idx = 2;
                if (ImGui::Combo("SphereTessel##combo", &sphTess_idx,  "16x32\0"\
                                                                       "8x16\0"\
                                                                       "4x8 (default)\0"\
                                                                       "2x4\0"\
                                )) 
                {
                    switch (sphTess_idx)
                    {
                        case imguiGizmo::sphereTess16: imguiGizmo::sphereTessFactor = imguiGizmo::sphereTess16; break;
                        case imguiGizmo::sphereTess8 : imguiGizmo::sphereTessFactor = imguiGizmo::sphereTess8 ; break;
                        case imguiGizmo::sphereTess4 : imguiGizmo::sphereTessFactor = imguiGizmo::sphereTess4 ; break;
                        case imguiGizmo::sphereTess2 : imguiGizmo::sphereTessFactor = imguiGizmo::sphereTess2 ; break;
                    }
                    needRebuild=true;
                }

                // sizeCylLength = defined in base to control size minus coneLenght

                if(needRebuild)  needRebuild = imguiGizmo::solidAreBuilded = false;


            } ImGui::EndGroup();


        } ImGui::End();
    }
    //  statistics
    ///////////////////////////////////////////
                
        if(metricW) ImGui::ShowMetricsWindow(&metricW);
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        if(demosW) ImGui::ShowDemoWindow(&demosW);



    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
