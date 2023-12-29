#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

int WhereIsObject(vector<float> vec, const float &obj_cx) {

    // Vector에서 일괄적으로 Value를 처리하는 방법
    transform(vec.begin(), vec.end(), vec.begin(),
                [obj_cx](float x) { return x - obj_cx; });

    // for (auto& x : vec) cout << x << endl;

    // Vector에서 조건을 만족하는 갯수를 뽑는 방법
    int minus_cnt = count_if(vec.begin(), vec.end(), [](float c){return c < 0;});

    return minus_cnt;
}

void Init_LineMeta(map<int, map<string, float>> &LineMetadata, const int &cnt_line) {
    for (int i=0; i<cnt_line; i++) {
        LineMetadata[i].insert(make_pair("IN", 0.0f));
        LineMetadata[i].insert(make_pair("OUT", 0.0f));
        LineMetadata[i].insert(make_pair("SUMINWEIGHT", 0.0f));
        LineMetadata[i].insert(make_pair("SUMPOWINWEIGHT", 0.0f));
        LineMetadata[i].insert(make_pair("SUMOUTWEIGHT", 0.0f));
        LineMetadata[i].insert(make_pair("SUMPOWOUTWEIGHT", 0.0f));
    }
}


int main() {

    // vector<float> line_vec = {300.0, 350.0, 400.0, 450.0, 500.0};

    vector<float> line_vec = {350.0, 400.0, 450.0};

    int linesize = line_vec.size();
    // Line통과 여유분
    float linespace = 5.0;

    map<int, map<string, float>> _LineMetadata;
    Init_LineMeta(_LineMetadata, linesize);

    vector<float> footprint = {512.0, 401.0, 512.0, 397.0, 512.0};
    // vector<float> footprint = {342.0, 402.0, 342.0, 398.0, 342.0, 407.0, 342.0};
    // vector<float> footprint = {342.0, 347.0, 358.0, 392.0, 399.0, 403.0, 407.0, 419.0, 449.0, 452.0, 457.0};


    int b_sector = -1;
    int a_sector = -1;

    for (const auto &now_footprint : footprint) {

        int now_sector = WhereIsObject(line_vec, now_footprint);

        if (b_sector < 0 && a_sector < 0) {
            b_sector = now_sector;
            a_sector = now_sector;
        } else if (b_sector >= 0 && a_sector >= 0) {

            b_sector = a_sector;

            if (b_sector != now_sector) {
                
                if (b_sector < now_sector) {
                    // 오른쪽으로 가는 경우(IN)
                    for (int i=std::min(b_sector, now_sector); i<std::max(b_sector, now_sector); i++){
                        if (line_vec[i] + linespace < now_footprint) {
                            _LineMetadata[i]["IN"] += 1;
                            a_sector = i + 1;
                        } 
                        else {
                            // 상황 연장
                            a_sector = i;
                        }
                    }
                    // cout << b_sector << " " << a_sector << " " << now_sector << endl;
                } else if (b_sector > now_sector) {
                    // 왼쪽으로 가는 경우 (OUT)
                    for (int i=std::max(b_sector, now_sector)-1; i>std::min(b_sector, now_sector)-1; i--) {
                        if (line_vec[i] - linespace > now_footprint) {
                            _LineMetadata[i]["OUT"] += 1;
                            a_sector = i;
                        } 
                        else {
                            a_sector = i + 1;
                        }
                    }
                }
            }
        }
        // cout << "[IN]LINE1 : " << _LineMetadata[1]["IN"] << endl;
        // cout << "[OUT]LINE1 : " << _LineMetadata[1]["OUT"] << endl;
    }

    // NOTE : 확인용 (제거)
    for (int i=0; i<linesize; i++) {
        cout << "[IN]LINE" << i << " : " << _LineMetadata[i]["IN"] << endl;
        cout << "[OUT]LINE" << i << " : " << _LineMetadata[i]["OUT"] << endl;
    }

}
