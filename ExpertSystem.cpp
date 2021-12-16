#include<iostream>
#include<string>
#include<fstream>
using namespace std;
// 事实最大数
#define MAXFACTS 100
// 规则最大数
#define MAXRELUNUMBER 20
// 规则前提条件数
#define MAXRELUCONDITION 10
// 输出事实
void printFact(string* facts, int length){
    for(int i = 1; i < length; i++){
        cout << i <<  "." << facts[i] << " ";
        if(i % 5 == 0)
            cout << endl;
    }
    cout << endl;
    cout << "------------------------------------------------------------" << endl;
}
// 判断是否包含目标元素
int contain(int *relu, int itemCount, int *inputFacts, int length){
    int i = 0;
    int count = 0;
    for(i = 0; i < itemCount; i++){
        for(int t = 0; t < length; t++){
            if(relu[i] == inputFacts[t]){
                count++;
                break;
            }
        }
    }
    if(count == itemCount)
        return true;
    else
        return false;
}
// 删除原条件
void conflict(int *relu, int itemCount, int *inputFacts, int length){
    for(int i = 0; i < itemCount; i++){
        for(int t = 0; t < length; t++){
            if(relu[i] == inputFacts[t]){
                inputFacts[t] = 0;
            }
        }
    }
}
// 增加推理结果
void addConclusion(int *inputFacts, int length, int *reluResult, int i){
    for(int t = 0; t < length; t++){
        if(inputFacts[t] == reluResult[i])
            break;
    }
    for(int t = 0; t < length; t++){
        if(inputFacts[t] == 0){
            inputFacts[t] = reluResult[i];
            break;
        }
    }
}
// 计算每条规则中条件个数
int calItemCount(int *relu, int maxReluCount){
    int count = 0;
    for(int i = 0; i < maxReluCount; i++)
        if(relu[i] != 0)
            count++;
    return count;
}
// 检查推理结果是否唯一
int judgeResult(int *inputFacts, int length){
    int count = 0;
    for(int t = 0; t < length; t++){
        if(inputFacts[t] != 0){
            count++;
            if(count > 1)
                return -1;
        }
    }
    return inputFacts[0];
}
// 输出当前事实库
void printCurrentFact(string *facts, int *inputFacts, int length){
    cout << "Current facts:";
    for(int i = 0; i < length; i++){
        if(inputFacts[i] != 0)
            cout << facts[inputFacts[i]] << " ";
    }
    cout << endl;
}
// 输出规则
void printUseRelu(int *relu, int itemCount, string *facts, int conclusion){
    cout << "Use relu: IF ";
    for(int i= 0; i < itemCount; i++){
        cout << facts[relu[i]];
        if(itemCount > 1 && i < itemCount - 1)
            cout << " AND ";
    }
    cout << " THEN " << facts[conclusion] << endl;
}
// 推理过程
int inference(int (*relu)[MAXRELUCONDITION], int reluNumber, int* reluResult, int *inputFacts, int length, string *facts){
    int itemCount;
    for(int i = 0; i < reluNumber; i++){
        itemCount = calItemCount(relu[i], MAXRELUCONDITION);
        if(contain(relu[i], itemCount, inputFacts, length)){
            printCurrentFact(facts, inputFacts, length);
            printUseRelu(relu[i], itemCount, facts, reluResult[i]);
            conflict(relu[i], itemCount, inputFacts, length);
            addConclusion(inputFacts, length, reluResult, i);
        }
    }
    int result = judgeResult(inputFacts, length);
    return result;
}
// 判断是否有重复事实
int judgeRepeatFact(string fact, string *facts, int factCount){
    for(int i = 0; i < factCount; i++)
        if(facts[i] == fact)
            return i;
    return 0;
}
// 读取文件
void readRelu(string fileName, string *facts, int (*relu)[MAXRELUCONDITION], int *reluResult, int &factCount, int &reluNumber){
    ifstream file;
    file.open(fileName);
    string lineData;
    string temp;
    int conditionNumber = 0;
    bool isConclusion = false;
    while(getline(file, lineData, '\n')){
        for(int i = 0; i < lineData.length(); i++){
            if(lineData[i] != ' ')
                temp += lineData[i];
            else if(temp == "IF" || temp == "AND")
                temp = "";
            else if(temp == "THEN"){
                temp = "";
                isConclusion = true;
            } else {
                int code = judgeRepeatFact(temp, facts, factCount);
                if(!isConclusion) {
                    if(code == 0){
                        facts[factCount] = temp;
                        relu[reluNumber][conditionNumber++] = factCount++;
                    } else {
                        relu[reluNumber][conditionNumber++] = code;
                    }
                } else if(isConclusion){
                    if(code == 0){
                        facts[factCount] = temp;
                        reluResult[reluNumber++] = factCount++;
                    } else {
                        reluResult[reluNumber++] = code;
                    }
                }
                temp = "";
            }
        }
        conditionNumber = 0;
        isConclusion = false;
    }
    file.close();
}
// 初始化
void initial(int (*relu)[MAXRELUCONDITION], int *reluResult){
    for(int i = 0; i < MAXRELUNUMBER; i++){
        // 结果
        reluResult[i] = 0;
        for(int j = 0; j < MAXRELUCONDITION; j++){
            // 规则
            relu[i][j] = 0;
        }
    }
}
int main(){
    // 事实数量
    int factCount = 1;
    // 规则数量
    int reluNumber = 0;
    // 事实
    string facts[MAXFACTS];
    // 规则条件
    int relu[MAXRELUNUMBER][MAXRELUCONDITION];
    // 规则结果
    int reluResult[MAXRELUNUMBER];
    string fileName = "animal.txt";
    // string fileName = "dataIll.txt";
    initial(relu, reluResult);
    readRelu(fileName, facts, relu, reluResult, factCount, reluNumber);
    cout << "Read " << fileName << " fact library" << endl;
    printFact(facts, factCount);
    // int inputFacts[2] = {3, 8};
    // // int length = 2;
    // // int inputFacts[6] = {3, 10, 11, 12, 16, 17};
    int inputFacts[10];
    int length = 0;
    cout << "enter the fact number:";
    char input[20];
    cin.get(input, 20);
    string temp;
    for(int i = 0; i < 20; i++){
        if(input[i] != ' '){
            temp += input[i];
        } else {
            inputFacts[length++] = atoi(temp.c_str());
            temp = "";
        }
        if(input[i] == 0){
            inputFacts[length++] = atoi(temp.c_str());
            break;
        }          
    }
    int result = inference(relu, reluNumber, reluResult, inputFacts, length, facts);
    if(result != -1)
        cout << "The conclusion of factual inference is:" << facts[result] << endl;
    else
        cout << "Inference failed" << endl;
    system("pause");
    return 0;
}