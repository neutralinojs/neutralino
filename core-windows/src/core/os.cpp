// MIT License

// Copyright (c) 2018 Neutralinojs

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <fstream>
#include "../../lib/json/json.hpp"
#include <windows.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "../settings.h"
#include "../functions.h"

using namespace std;
using json = nlohmann::json;

namespace os {

    string runCommand(string jso) {
        json input;
        json output;
        try {
            input = json::parse(jso);
        }
        catch(exception e){
            output["error"] = "JSON parse error is occurred!";
            return output.dump();
        }
        
        string command = "cmd /c " + input["command"].get<std::string>();

        PROCESS_INFORMATION pi;
        STARTUPINFO si = {sizeof(si)};
        char temp[256];
        GetTempPathA(256, temp);
        string tmpFile = string(temp) + "nl_o" + functions::generateToken(4) + ".tmp";
        CreateProcessA(NULL,(LPSTR)(command + " > " + tmpFile).c_str(),NULL,NULL,TRUE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        output["stdout"] = settings::getFileContent(tmpFile);
        DeleteFile(tmpFile.c_str());
        return output.dump();

    }

    string getEnvar(string jso) {
        json input;
        json output;
        try {
            input = json::parse(jso);
        }
        catch(exception e){
            output["error"] = "JSON parse error is occurred!";
            return output.dump();
        }
        string i = input["name"];
        char *o;
        o = getenv(i.c_str());
        if(o == NULL) {
            output["error"] =  i + " is not defined";
        }
        else {
            output["value"] = o;
        }
        return output.dump();
       
        
    }

    
}
