#pragma once

#include <d3d12.h>
#include <map>
#include <string>
#include <iostream>

//Similar to how it is in Ry-Engine with our D3D11 renderer, D3D12Core will store any necessary Direct3D stuff to initialize as well as hold any debugging stuff

namespace Ry_Engine
{
    bool CheckHResult(HRESULT hresult, std::string_view statement, std::string_view file, uint32_t line);

#if RYAPP_DEBUG
#define DX_CALL(func)\
            {\
                HRESULT r = func;\
                CheckHResult((r), #func, __FILE__, __LINE__ );\
                if (!SUCCEEDED(r))\
                {\
                    std::cout << " DirectX12 Error!" << std::endl;\
                }\
            }
#else
#define DX_CALL(func) func
#endif
}