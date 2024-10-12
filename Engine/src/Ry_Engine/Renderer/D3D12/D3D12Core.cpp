#include "Ry-Engine_PCH.h"
#include "D3D12Core.h"

namespace Ry_Engine
{
	bool CheckHResult(HRESULT hresult, std::string_view statement, std::string_view file, uint32_t line)
	{
		if (FAILED(hresult))
		{
			LPSTR buffer;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hresult, 0, reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
			//printf("%s %d failed with error: %s", file.data(), line, statement.data());
			std::cout << file.data() << " at line " << line << " failed with error: " << statement.data() << std::endl;
			return false;
		}
		return true;
	}
}