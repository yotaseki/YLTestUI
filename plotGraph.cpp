#include "plotGraph.h"
using namespace std;

PlotGraph::PlotGraph(){
	gp = popen("gnuplot -persist", "w");
}
PlotGraph::PlotGraph(vector<vector<double>> &mat){
	data = mat;
	gp = popen("gnuplot -persist", "w");
}

PlotGraph::~PlotGraph(){
	fprintf(gp,"set nomultiplot\n");
	fprintf(gp, "exit\n");
	fflush(gp);
	pclose(gp);
}
int PlotGraph::mat2vec(int index, int axis, vector<double> &vec){
	
    int cols = data[0].size();
    int rows = data.size();
	if(axis == 0){
		if(rows< index){
			cout << "[ERROR]index is large than data rows" << endl;
			return 1;
		}
		vec.resize(cols);
		for(int i=0; i< cols; i++){
			vec[i] = data[index][i];
		}
	}
	else if(axis == 1){
		if(cols < index){
			cout << "[ERROR]index is large than data cols" << endl;
			return 1;
		}
		vec.resize(rows);
		for(int i=0; i< rows; i++){
			vec[i] = data[i][index];
		}
	}
	else{
		cout << "[ERROR]invalid axis" << endl;
		return 1;
	}
	return 0;
}

int PlotGraph::set_range(double min,double max,int axis){
    std::string buf;
    if(axis=0){
        buf = "set xrange [" + std::to_string(min) + ":"+ std::to_string(max) + "]\n";
    }
    else if(axis=1){
        buf = "set yrange [" + std::to_string(min) + ":"+ std::to_string(max) + "]\n";
    }
    else{
		cout << "[ERROR]invalid axis" << endl;
        return 1;
    }
    fprintf(gp, buf.c_str());
    return 0;
}

int PlotGraph::plot(std::vector<double> &y,std::vector<double> &x, std::string &filename){
	fprintf(gp, "set terminal png\n");
    std::string out_buf = "set output '" + filename + "'\n";
    //fprintf(gp, "set output 'graph.png'\n");
    fprintf(gp, out_buf.c_str());
	fprintf(gp, "set multiplot\n");
	fprintf(gp, "set xlabel \"Recall\"\n");
	fprintf(gp, "set ylabel \"Precision\"\n");
	fprintf(gp, "plot '-' with lines linetype 1 linewidth 3\n");
	for (int i = 0; i < x.size(); ++i) {
		fprintf(gp, "%f\t%f\n", x[i], y[i]);
	}
	fprintf(gp, "e\n");
}
