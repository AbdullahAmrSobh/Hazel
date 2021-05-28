print("Searching for VulkanSDK")

local vulkanSdkLocation = os.getenv("VULKAN_SDK")

local function file_exists(name)
    local f=io.open(name,"r")
    if f~=nil then io.close(f) return true else return false end
end

local function getTempPath()
    local directorySeperator = package.config:match("([^\n]*)\n?")
    local exampleTempFilePath = os.tmpname()
    
    -- remove generated temp file
    pcall(os.remove, exampleTempFilePath)

    local seperatorIdx = exampleTempFilePath:reverse():find(directorySeperator)
    local tempPathStringLength = #exampleTempFilePath - seperatorIdx

    return exampleTempFilePath:sub(1, tempPathStringLength)
end



if not vulkanSdkLocation then 
    print("Vulkan SDK not installed")

    local sdkPath = getTempPath() .. "\\vulkan-sdk.exe"

    if not file_exists(sdkPath .. "/vulkan-sdk.exe") then
        print("Downloading VulkanSDK")
        http.download("https://sdk.lunarg.com/sdk/download/latest/windows/vulkan-sdk.exe", sdkPath)
    end
    
    print("Installing VulkanSDK")

    if os.execute(sdkPath) then 
        print("VulkanSDK Installed successfully")
    else
        print("Failed to install the sdk")
        os.remove(sdkPath)
        os.exit()
    end
    
else
    print("VulkanSDK Found")
end
