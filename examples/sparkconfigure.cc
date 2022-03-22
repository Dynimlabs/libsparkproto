// Copyright 2021 Dynim Oy.
// SPDX-License-Identifier: BSD 3-Clause


/**
 * Example of a simple configuration tool for get/set device/sensor parameters
 * 
 */

#include <getopt.h>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <libsparkproto/sparkproto.h>

using namespace libspark::protocol;

class Commandline {

public:
    Commandline() = default;

    void add(const char* name, const char* help) {
        struct option op;
        op.name = name;
        op.has_arg = required_argument;
        op.flag = 0;
        op.val = 0;

        _options.push_back(op);
        _mapHelp.insert({std::string{name} + " <value>", std::string{help}});
    }

    void addBool(const char* name, const char* help) {
        struct option op;
        op.name = name;
        op.has_arg = no_argument;
        op.flag = 0;
        op.val = 0;

        _options.push_back(op);
        _mapHelp.insert({std::string{name}, std::string{help}});
    }

    template<typename T>
    bool get(const char* name, T &value) {
        if(_map.find(name) != _map.end()){
            std::stringstream convert(_map[name]);
            // TODO: input can be invalid value
            convert >> value;
            return true;
        }
        else{
            return false;
        }
    }

    bool parse(int argc, char** argv){
        _options.push_back({"help", no_argument, 0, 'h'});
        _options.push_back({0, 0, 0, 0});
        _optString += "h";
        _mapHelp.insert({"help, -h", "Print help"});

        if(argc < 2){
            printHelp(argv, std::cout);
        }

        int option_index = 0;
        int c;
        while(-1 != (c = getopt_long(argc, argv, _optString.c_str(), &_options[0], &option_index))) {
            switch (c)
            {
            case 'h':
                printHelp(argv, std::cout);
                break;
            
            case 0:
                if(optarg)
                    _map[_options[option_index].name] = std::string{optarg};
                else
                    _map[_options[option_index].name] = std::to_string(true);
                break;
            default:
                printHelp(argv, std::cout);
                break;
            }
        }
    }

    void printHelp(char** argv, std::ostream & out){
        out << "Usage: "<< argv[0] <<" [options]" <<std::endl;
        for(auto & it : _mapHelp){
            out << "  --" << it.first << "\t\t" << it.second<< std::endl;
        }

        // force exit
        exit(1);
    }

private:
    std::string _optString;
    std::vector<struct option> _options;
    std::map<std::string, std::string> _map;
    std::map<std::string, std::string> _mapHelp;
};

static void configCmdInput(Commandline *cmd) {
    cmd->addBool("discover", "Discovers new devices on network");
    cmd->addBool("info", "Print the information of a given device)");
    cmd->add("dev", "Select device by ID to request, default device 0 will be selected (int)");
    cmd->addBool("all", "Request to print all available parameters");
    cmd->add("autoexp", "Enable or disable auto exposure mode (true/false)");
    cmd->add("autoexp_mode", "Set auto exposure mode (1)");
    cmd->add("exp_val", "Set exposure value (6-36200)");
    cmd->add("gain_val", "Set gain value (0-480)");
    cmd->add("autowb", "Enable or disable auto whitebalance mode (true/false)");
    cmd->add("autowb_mode", "Set autowb mode (1)");
    cmd->add("mwb_mode", "Set whitebalance mode (0-5)");
    cmd->add("led", "Turn on or off LED (true/false)");
    cmd->add("led_brightness", "Set brightness level of led (0-3)");
    cmd->add("export_calib", "Export calibration data to a file (string)");
}

struct RequestParameters {
    bool discoverDev;
    // dev selected
    int dev;
    // devinfo
    bool devinfo;
    // request all parameters
    bool requestAllParam;
    // exposure param
    bool enableAutoExp;
    int autoExpMod;
    int manualExpVal;
    // gain
    int manualGainVal;
    // whitebalance
    bool enableAutoWb;
    int autoWbMod;
    int manualWbVal;
    // led
    bool enableLed;
    int ledBrightnessLevel;
    // export calibration
    std::string calibOutputPath;
} request;

int main(int argc, char **argv) {

    Commandline cmd;
    configCmdInput(&cmd);
    cmd.parse(argc, argv);

    std::shared_ptr<DeviceInfo> dev;
    
    std::unique_ptr<DeviceEnumeration> deviceEnum = std::make_unique<DeviceEnumeration>();
    DeviceList deviceList = deviceEnum->discoverDevices();


    if(deviceList.empty()){
        std::cout<<"No device found!!"<<std::endl;
        return 1;
    }

    if(cmd.get("discover", request.discoverDev)){
        int index = 0;
        for(auto &it : deviceList){
            std::cout<<"\n>>>Device "<< index++ <<":"<<std::endl;
            it->printDebugString();
        }
        return 0;
    }
    
    if(cmd.get("dev", request.dev)){
        if(request.dev >= deviceList.size()){
            std::cout<<"value of dev should be: 0-"<<deviceList.size() - 1 <<std::endl;
            return 1;
        }
        dev = deviceList[request.dev];
    }
    else{
        dev = deviceList[0];
    }

    try {

        DeviceParamConfigure configure(dev);

        if(cmd.get("autoexp", request.enableAutoExp)){
            configure.setAutoExposure(request.enableAutoExp);
            std::cout<<"AutoExposure: "<<configure.getAutoExposure()<<std::endl;
            return 0;
        }

        if(cmd.get("autoexp_mode", request.autoExpMod)){
            configure.setAutoExposureMode(request.autoExpMod);
            std::cout<<"AutoExposureMode: "<<configure.getAutoExposureMode()<<std::endl;
            return 0;
        }

        if(cmd.get("exp_val", request.manualExpVal)){
            configure.setExposureValue(request.manualExpVal);
            std::cout<<"ExposureValue: "<<configure.getExposureValue()<<std::endl;
            return 0;
        }

        if(cmd.get("gain_val", request.manualGainVal)){
            configure.setGainValue(request.manualGainVal);
            std::cout<<"GainValue: "<<configure.getGainValue()<<std::endl;
            return 0;
        }

        if(cmd.get("autowb", request.enableAutoWb)){
            configure.setAutoWhiteBalance(request.enableAutoWb);
            std::cout<<"AutoWB: "<<configure.getAutoWhiteBalance()<<std::endl;
            return 0;
        }

        if(cmd.get("autowb_mode", request.autoWbMod)){
            configure.setAutoWhiteBalanceMode(request.autoWbMod);
            std::cout<<"AutoWBMode: "<<configure.getAutoWhiteBalanceMode()<<std::endl;
            return 0;
        }

        if(cmd.get("mwb_mode", request.manualWbVal)){
            configure.setWhiteBalanceValue(request.manualWbVal);
            std::cout<<"WB val: "<<configure.getWhiteBalanceValue()<<std::endl;
            return 0;
        }

        if(cmd.get("led", request.enableLed)){
            configure.setLedMode(request.enableLed);
            std::cout<<"LedMod: "<<configure.getLedMode()<<std::endl;
            return 0;
        }
        
        if(cmd.get("led_brightness", request.ledBrightnessLevel)){
            configure.setLedBrightnessLevel(request.ledBrightnessLevel);
            std::cout<<"LedBrightnessLevel: "<<configure.getLedBrightnessLevel()<<std::endl;
            return 0;
        }

        if(cmd.get("export_calib", request.calibOutputPath)){
            configure.exportCalibrationData(request.calibOutputPath);
            std::cout<<"Exported calibration data to: "<<request.calibOutputPath<<std::endl;
            return 0;
        }

        if(cmd.get("info", request.devinfo)){
            DeviceInfoMessage devInfoMsg;
            configure.readDeviceInfoMsg(devInfoMsg);
            devInfoMsg.PrintDebugString();
            return 0;
        }

        if(cmd.get("all", request.requestAllParam)){
            std::cout<<"AutoExposure: "<<configure.getAutoExposure()<<std::endl;
            std::cout<<"AutoExposureMode: "<<configure.getAutoExposureMode()<<std::endl;
            std::cout<<"ExposureValue: "<<configure.getExposureValue()<<std::endl;
            std::cout<<"AutoWB: "<<configure.getAutoWhiteBalance()<<std::endl;
            std::cout<<"AutoWBMode: "<<configure.getAutoWhiteBalanceMode()<<std::endl;
            std::cout<<"WBValue: "<<configure.getWhiteBalanceValue()<<std::endl;
            std::cout<<"Resolution index: "<<configure.getResolution()<<std::endl;
            std::cout<<"GainValue: "<<configure.getGainValue()<<std::endl;
            std::cout<<"LedMode: "<<configure.getLedMode()<<std::endl;
            std::cout<<"LedBrightnessLevel: "<<configure.getLedBrightnessLevel()<<std::endl;
        }

    } catch (SparkException &e){
        std::cout<<e.toString()<<std::endl;
        return 1;
    }
    
    return 0;
}
