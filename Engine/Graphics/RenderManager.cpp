#include "RenderManager.h"

#include "Core/ShaderManager.h"
#include "DefaultTextures.h"
#include "GameWindow.h"
#include "ImGuiManager.h"


#include "Framework/Engine.h"

static bool useGrayscale = true;

RenderManager* RenderManager::GetInstance() {
    static RenderManager instance;
    return &instance;
}

void RenderManager::Initialize() {
    graphics_ = Graphics::GetInstance();

    auto shaderManager = ShaderManager::GetInstance();
    shaderManager->Initialize();
    shaderManager->SetDirectory(std::filesystem::current_path() / SHADER_DIRECTORY);

    auto window = GameWindow::GetInstance();
    swapChain_.Create(window->GetHWND());

    DefaultTexture::Initialize();

    auto& swapChainBuffer = swapChain_.GetColorBuffer(0);
    finalImageBuffer_.Create(L"FinalImageBuffer", swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight(), swapChainBuffer.GetFormat());

    skinningManager_.Initialize();
    geometryRenderingPass_.Initialize(swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight());
    lightingRenderingPass_.Initialize(swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight());
    //skybox_.Initialize(lightingRenderingPass_.GetResult().GetRTVFormat(), geometryRenderingPass_.GetDepth().GetFormat());
    lineDrawer_.Initialize(lightingRenderingPass_.GetResult().GetRTVFormat());
    //particleCore_.Initialize(lightingRenderingPass_.GetResult().GetRTVFormat());

    bloom_.Initialize(&lightingRenderingPass_.GetResult());
    fxaa_.Initialize(&lightingRenderingPass_.GetResult());
    postEffect_.Initialize(finalImageBuffer_);

    //    modelRenderer.Initialize(mainColorBuffer_, mainDepthBuffer_);
    transition_.Initialize();
    
    //testRTRenderer_.Create(swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight());
    //raytracingRenderer_.Create(swapChainBuffer.GetWidth(), swapChainBuffer.GetHeight());

    //raymarchingRenderer_.Create(mainColorBuffer_.GetWidth(), mainColorBuffer_.GetHeight());

    //computeShaderTester_.Initialize(1024, 1024);
    //commandContext_.Start(D3D12_COMMAND_LIST_TYPE_DIRECT);
    //computeShaderTester_.Dispatch(commandContext_);
    //commandContext_.Finish(true);

    auto imguiManager = ImGuiManager::GetInstance();
    imguiManager->Initialize(window->GetHWND(), finalImageBuffer_.GetRTVFormat());
    imguiManager->NewFrame();

    timer_.Initialize();
    skyTexture_.Create(L"SkyTexture", lightingRenderingPass_.GetResult().GetWidth(), lightingRenderingPass_.GetResult().GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM);
    skyRenderer_.Initialize(skyTexture_.GetRTVFormat());

    particleRenderer_.Initialize(lightingRenderingPass_.GetResult(), geometryRenderingPass_.GetDepth());
   
    frameCount_ = 0;

}

void RenderManager::Finalize() {
    auto imguiManager = ImGuiManager::GetInstance();
    imguiManager->Shutdown();


    DefaultTexture::Finalize();
}

void RenderManager::Render() {

    uint32_t targetSwapChainBufferIndex = (swapChain_.GetCurrentBackBufferIndex() + 1) % SwapChain::kNumBuffers;

    auto camera = camera_.lock();
    auto sunLight = sunLight_.lock();

    commandContext_.Start(D3D12_COMMAND_LIST_TYPE_DIRECT);

    //const float deltaSecond = 1 / 60.0f;
    //const float daySpeed = 1;
    //sky_.Update(deltaSecond / daySpeed);

    skinningManager_.Update(commandContext_);

    //particleCore_.Dispatch(commandContext_);

    //空
    commandContext_.TransitionResource(skyTexture_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext_.SetRenderTarget(skyTexture_.GetRTV());
    commandContext_.SetViewportAndScissorRect(0, 0, skyTexture_.GetWidth(), skyTexture_.GetHeight());
    float skyScale = camera->GetFarClip() * 2.0f;
    skyRenderer_.Render(commandContext_, *camera, Matrix4x4::MakeAffineTransform({ skyScale, skyScale, skyScale }, Quaternion::identity, camera->GetPosition()));

    if (camera && sunLight) {
        // 影、スペキュラ
        modelSorter_.Sort(*camera);

        //raytracingRenderer_.Render(commandContext_, *camera, *sunLight);
        //testRTRenderer_.Render(commandContext_, *camera, modelSorter_);

        geometryRenderingPass_.Render(commandContext_, *camera, modelSorter_);
        lightingRenderingPass_.Render(commandContext_, geometryRenderingPass_, *camera, *sunLight, skyTexture_);

        commandContext_.TransitionResource(lightingRenderingPass_.GetResult(), D3D12_RESOURCE_STATE_RENDER_TARGET);
        commandContext_.TransitionResource(geometryRenderingPass_.GetDepth(), D3D12_RESOURCE_STATE_DEPTH_READ);
        commandContext_.SetViewportAndScissorRect(0, 0, lightingRenderingPass_.GetResult().GetWidth(), lightingRenderingPass_.GetResult().GetHeight());
        commandContext_.SetRenderTarget(lightingRenderingPass_.GetResult().GetRTV(), geometryRenderingPass_.GetDepth().GetDSV());
  


        particleRenderer_.Render(commandContext_, *camera);

        commandContext_.SetRenderTarget(lightingRenderingPass_.GetResult().GetRTV());
        commandContext_.SetViewportAndScissorRect(0, 0, lightingRenderingPass_.GetResult().GetWidth(), lightingRenderingPass_.GetResult().GetHeight());
        lineDrawer_.Render(commandContext_, *camera);

        //particleCore_.Render(commandContext_, *camera);



     

    }

   

    bloom_.Render(commandContext_);
    fxaa_.Render(commandContext_);

    commandContext_.TransitionResource(finalImageBuffer_, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandContext_.SetRenderTarget(finalImageBuffer_.GetRTV());
    commandContext_.SetViewportAndScissorRect(0, 0, finalImageBuffer_.GetWidth(), finalImageBuffer_.GetHeight());

    postEffect_.Render(commandContext_, fxaa_.GetResult());

    auto imguiManager = ImGuiManager::GetInstance();
    imguiManager->Render(commandContext_);

    auto& swapChainBuffer = swapChain_.GetColorBuffer(targetSwapChainBufferIndex);

    commandContext_.CopyBuffer(swapChainBuffer, finalImageBuffer_);

    // ImGuiを描画

    commandContext_.TransitionResource(swapChainBuffer, D3D12_RESOURCE_STATE_PRESENT);

    // コマンドリスト完成(クローズ)
    commandContext_.Close();

    // バックバッファをフリップ
    swapChain_.Present();
    frameCount_++;
    // シグナルを発行し待つ
    auto& commandQueue = graphics_->GetCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
    commandQueue.WaitForIdle();

    commandContext_.Finish(false);

    graphics_->GetReleasedObjectTracker().FrameIncrementForRelease();


    timer_.KeepFrameRate(60);
    imguiManager->NewFrame();


}