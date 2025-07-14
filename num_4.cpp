#include <iostream>
#include <cctype>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//нужно создать метод, высчитывающий средний былл
//с учетом того, что двойки туда не входят
double sum(const vector<double> &array, int i = 0){
    if(i >= array.size()){
        return 0.0;
    }
    if(array[i] == 2){
        return sum(array, i+1);
    }
    return array[i] + sum(array, i+1);
}

int count_not2(const vector<double> &array, int i = 0){
    if(i >= array.size()){
        return 0;
    }
    if(array[i] == 2){
        return count_not2(array, i+1);
    }
    return 1 + count_not2(array, i+1);
}

double average_sum(const vector<double> &array){
    int n = count_not2(array);
    if(n == 0){
        return 0.0;
    }
    return sum(array)/n;
}

bool has_2(const vector<double> &array) {
    for (double x : array){
        if (x == 2) return true;
    }
    return false;
}

struct Student{
    char last_name[100];
    int num_exams;
    vector<double> grades;
};

int main(){
    vector<Student> students;

    try{
        ifstream finput("num_4.txt");
        if(!finput){
            throw runtime_error("Error");
        }

        string line;
        while(getline(finput, line)){
            istringstream str_stream(line);
            Student stud;
            str_stream >> stud.last_name >> stud.num_exams;
            string gr;
            for(int i = 0; i < stud.num_exams; ++i){
                if(!(str_stream >> gr)){
                    throw runtime_error("error");
                }
                if(!isdigit(gr[i])){
                    throw runtime_error("error2");
                }
                double grade = stod(gr);
                stud.grades.push_back(grade);
            }
            students.push_back(stud);
        }
        finput.close();

        //групповой средний балл
        double gr_sum = 0.0;
        int gr_count = 0;
        for(const Student &st : students){
            for(double x : st.grades){
                if(x!=2){
                    gr_sum += x;
                    gr_count++;
                }
            }
        }
        if(gr_count == 0){
            throw runtime_error("error");
        }
        double gr_avg = gr_sum/gr_count;

        ofstream fout("total.txt");
        if(!fout){
            throw runtime_error("error");
        }
        for(const Student &st : students){
            double avg = average_sum(st.grades);
            if(!has_2(st.grades) && avg > gr_avg){
                fout << st.last_name << endl;
            }
        }
        fout.close();
    }catch(const exception &err){
        cerr << "error: " << err.what() << endl;
        return 1;
    }
}