#include "slicer.hpp"

// Constructors
Slicer::Slicer()
{
}

Slicer::Slicer(float layerHeight, int bedTemp, int nozzleTemp) : layerHeight(layerHeight), bedTemp(bedTemp), nozzleTemp(nozzleTemp)
{
}

// Public Methods
void Slicer::test()
{
    std::ofstream myFile("outputTest.txt");

    myFile << "Files can be tricky, but it is fun enough!";

    myFile.close();
}

// Private Methods
void Slicer::makeGcodeInfo(std::vector<std::string> &strBuff)
{
    time_t now = time(0);

    tm *ltm = localtime(&now);

    std::string year = std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(1 + ltm->tm_mon);
    std::string day = std::to_string(ltm->tm_mday);

    strBuff.push_back("; " + year + month + day + '\n');
}

void Slicer::makeGcodeHeatSettings(std::vector<std::string> &strBuff)
{
    strBuff.push_back("M140 S" + std::to_string(bedTemp) + " ; Set Bed Temperature\n");
    strBuff.push_back("M105\n");
    strBuff.push_back("M190 S" + std::to_string(bedTemp) + " ; Wait for Bed Temperature\n");
    strBuff.push_back("M104 S" + std::to_string(nozzleTemp) + " ; Set Nozzle Temperature\n");
    strBuff.push_back("M105\n");
    strBuff.push_back("M109 S" + std::to_string(nozzleTemp) + " ; Wait for Nozzle Temperature\n");
}

void Slicer::makeGcodeStartupSettings(std::vector<std::string> &strBuff)
{
    strBuff.push_back("G28\n");
    strBuff.push_back("M83\n");
    strBuff.push_back("G90\n");
    strBuff.push_back("G1 F300 Z0.4\n");
    strBuff.push_back("G1 X50 E8 F800\n");
}

void Slicer::writeFile(std::string str, std::string path)
{
    std::ofstream myFile(path + "output.txt");
    myFile << str;
    myFile.close();
}

std::string Slicer::makeRetraction(float amount, float speed, int sign)
{
    std::string message = "";
    if (sign == -1)
    {
        message = " ; Retraction";
    }
    else
    {
        message = " ; Extraction";
    }

    return "G1 F" + std::to_string(speed) + " E" + std::to_string(sign * amount) + message + "\n";
}

std::string Slicer::makeGcodePoints(glm::vec2 from, glm::vec2 to)
{
    float length = glm::distance(to, from);
    float numerator = nozzleWidth * length * layerHeight;
    float denominator = (1.75 / 2) * (1.75 / 2) * M_PI;
    float e = numerator / denominator;

    // Not sure if this is going to work
    // I'll need to verify.

    std::setprecision(2);
    std::string tox_fmt = std::to_string(to.x);
    std::string toy_fmt = std::to_string(to.y);

    std::setprecision(8);
    std::string e_fmt = std::to_string(e);

    return "G1 F1200 X" + tox_fmt + " Y" + toy_fmt + " E" + e_fmt + '\n';
}

std::string Slicer::makeGcodeSpeed(glm::vec2 from, glm::vec2 to, float speed)
{
    float length = glm::distance(to, from);
    float numerator = nozzleWidth * length * layerHeight;
    float denominator = (1.75 / 2) * (1.75 / 2) * M_PI;
    float e = numerator / denominator;

    // Not sure if this is going to work
    // I'll need to verify.

    std::setprecision(2);
    std::string tox_fmt = std::to_string(to.x);
    std::string toy_fmt = std::to_string(to.y);

    std::setprecision(8);
    std::string e_fmt = std::to_string(e);

    return "G1 F" + std::to_string(speed) + " X" + tox_fmt + " Y" + toy_fmt + " E" + e_fmt + '\n';
}

std::string Slicer::makeGcode(glm::vec2 to)
{
    std::setprecision(2);
    std::string tox_fmt = std::to_string(to.x);
    std::string toy_fmt = std::to_string(to.y);

    return "G0 F9000 X" + tox_fmt + " Y" + toy_fmt + '\n';
}

void Slicer::apply()
{
    std::vector<std::string> stringBuffer;

    makeGcodeInfo(stringBuffer);
    makeGcodeHeatSettings(stringBuffer);
    makeGcodeStartupSettings(stringBuffer);
    stringBuffer.push_back(makeRetraction(retAmount, retSpeed, -1));

    float currentHeight = 0;

    // Pillar generation
    int gap = 3;
    int layerNum = 0;
    int hairNum = 0;

    /*
    for (int i = 0; i < i in range(0, len(MeshPoints.Paths)):
        pList = MeshPoints.Branch(i)
        
        if(len(pList) != 0):
            CurrentHeight = pList[0].Z
            sb.append("G1 F300 Z" + str(CurrentHeight) + "\n")
            sb.append(makeGcode(pList[0]))
            sb.append(makeRetraction(_RetAmount, _RetSpeed, 1))
    
            j = 0
            for j in range(0,len(pList)):
                if(j == 0):
                    #sb.Append("G4 P100\n");
                    sb.append(makeGcode(pList[j]))
                    continue

                print(LayerHeight)
                if(pList[j].Z > CurrentHeight + (_LayerHeight / 10)):
                    CurrentHeight = pList[j].Z
                    sb.append("G1 F300 Z" + "{:.{}f}".format(CurrentHeight,3) + "\n")

                sb.append(makeGcodeSpeed(pList[j - 1], pList[j], PrintSpeed))

            sb.append(makeGcodeSpeed(pList[len(pList) - 1], pList[0], PrintSpeed))

        sb.append(makeRetraction(_RetAmount, _RetSpeed, -1))





    sb.append(makeRetraction(_RetAmount, _RetSpeed, -1));
    CurrentHeight += 10;
    sb.append("G1 Z" + "{:.{}f}".format(CurrentHeight,3) + "\n");


    sb.append("M84");
    strsb = ''.join(sb)
    WriteFile(strsb, FilePath);  
        
        
    a = strsb
    */
}