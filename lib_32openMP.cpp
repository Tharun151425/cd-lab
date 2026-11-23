// 1a - openmp 
#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

int main() {
    int n;
    cout << "Enter trials: ";
    cin >> n;
    cout << "Threads\tTime(s)\t\tPi\n";

    int threads[] = {1, 2, 4, 8};
    for (int th : threads) {
        int count = 0;
        omp_set_num_threads(th);
        double t = omp_get_wtime();

        srand(3655942);
        #pragma omp parallel for reduction(+:count)
        for (int i = 0; i < n; i++) {
            double x = (double)rand() / RAND_MAX;
            double y = (double)rand() / RAND_MAX;
            if (x * x + y * y <= 1.0) count++;
        }

        double pi = 4.0 * count / n;
        cout << th << "\t" << omp_get_wtime() - t << "\t" << pi << endl;
    }
}

//1b - mpi
#include <iostream>
#include <cstdlib>
#include <mpi.h>

#define SEED 3655942

#define ROOT 0

using namespace std;

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double t;
	if (rank == ROOT) {
		t = MPI_Wtime();
	}

	int n = 10000;
	int count = 0, red_count;
	srand(SEED);
	for (int i = 0; i < n / size; i++) {
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		double z = x*x + y*y;
		if (z <= 1.0) count++;
	}
	MPI_Reduce(&count, &red_count, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

	if (rank == ROOT) {
		t = MPI_Wtime() - t;
		cout << "Time: " << t << endl;

		double pi = (double)red_count / n * 4;
		cout << "no. of threads: " << size << endl;
		cout << "no. of trials: " << n << endl;
		cout << "est. of pi: " << pi << endl;
	}

	MPI_Finalize();
}

//2_matmul
#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

int main() {
    int n;
    cout << "Enter matrix size: ";
    cin >> n;

    int *A = new int[n * n];
    int *B = new int[n * n];
    int *C = new int[n * n];

    for (int i = 0; i < n * n; i++) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    cout << "Threads\tTime(s)\n";
    int threads[] = {1, 2, 4, 8};
    for (int th : threads) {
        for (int i = 0; i < n * n; i++) C[i] = 0;
        double t = omp_get_wtime();

        #pragma omp parallel for num_threads(th)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    C[n * i + k] += A[n * i + j] * B[n * j + k];

        cout << th << "\t" << omp_get_wtime() - t << endl;
    }

    delete[] A;
    delete[] B;
    delete[] C;
}


//3_sieve
#include <iostream>
#include <cmath>
#include <algorithm>
#include <omp.h>
using namespace std;

int strike(bool comp[], int start, int step, int stop) {
    int i;
    for (i = start; i <= stop; i += step)
        comp[i] = true;
    return i;
}

void unfriendlySieve(int n) {
    int m = sqrt(n), count = 0;
    bool *comp = new bool[n + 1]();
    double t = omp_get_wtime();

    for (int i = 2; i <= m; i++)
        if (!comp[i]) {
            count++;
            strike(comp, 2 * i, i, n);
        }
    for (int i = m + 1; i <= n; i++)
        if (!comp[i]) count++;

    cout << "Unfriendly -> Count: " << count << " | Time: " << omp_get_wtime() - t << " s\n";
    delete[] comp;
}

void friendlySieve(int n, bool parallel) {
    int m = sqrt(n), count = 0, fac_cnt = 0;
    bool *comp = new bool[n + 1]();
    int *factor = new int[m], *striker = new int[m];
    double t = omp_get_wtime();

    for (int i = 2; i <= m; i++)
        if (!comp[i]) {
            count++;
            factor[fac_cnt] = i;
            striker[fac_cnt++] = strike(comp, 2 * i, i, m);
        }

    for (int left = m + 1; left <= n; left += m) {
        int right = min(left + m - 1, n);

        #pragma omp parallel for if(parallel)
        for (int k = 0; k < fac_cnt; k++)
            striker[k] = strike(comp, striker[k], factor[k], right);

        #pragma omp parallel for reduction(+:count) if(parallel)
        for (int i = left; i <= right; i++)
            if (!comp[i]) count++;
    }

    cout << (parallel ? "Parallel   -> " : "Friendly   -> ")
         << "Count: " << count << " | Time: " << omp_get_wtime() - t << " s\n";
    delete[] comp;
    delete[] factor;
    delete[] striker;
}

int main() {
    int n = 100000000;
    unfriendlySieve(n);
    friendlySieve(n, false);
    friendlySieve(n, true);
}

//4_img_complex
#include <iostream>
#include <cstdio>
#include <gd.h>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) { cout << "Usage: ./4 input.png\n"; return 1; }

    FILE *ifp = fopen(argv[1], "rb");
    gdImagePtr img = gdImageCreateFromPng(ifp);
    fclose(ifp);

    int w = gdImageSX(img), h = gdImageSY(img);
    gdImagePtr bw  = gdImageCreateTrueColor(w, h);
    gdImagePtr pat = gdImageCreateTrueColor(w, h);

    int tCol[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00}; // Red, Green, Blue, Yellow
    omp_sched_t scheds[] = {omp_sched_static, omp_sched_dynamic, omp_sched_guided};
    const char *names[]  = {"static", "dynamic", "guided"};
    const char *files[]  = {"static.png", "dynamic.png", "guided.png"};
    int chunks[] = {100, 50, 10};

    cout << "Sched\tChunk\tTime(s)\n";
    for (int s = 0; s < 3; s++) {
        for (int c : chunks) {
            omp_set_schedule(scheds[s], c);
            double t = omp_get_wtime();

            #pragma omp parallel for num_threads(4) schedule(runtime)
            for (int x = 0; x < w; x++) {
                int tid = omp_get_thread_num();
                for (int y = 0; y < h; y++) {
                    int p = gdImageGetPixel(img, x, y);
                    int avg = (gdImageRed(img, p) + gdImageGreen(img, p) + gdImageBlue(img, p)) / 3;
                    gdImageSetPixel(bw, x, y, gdImageColorAllocate(bw, avg, avg, avg));
                    gdImageSetPixel(pat, x, y, tCol[tid]);
                }
            }
            cout << names[s] << "\t" << c << "\t" << omp_get_wtime() - t << endl;
        }
        FILE *pf = fopen(files[s], "wb");
        gdImagePng(pat, pf);
        fclose(pf);
    }

    FILE *ofp = fopen("bw.png", "wb");
    gdImagePng(bw, ofp);
    fclose(ofp);

    gdImageDestroy(img);
    gdImageDestroy(bw);
    gdImageDestroy(pat);
}


//4_image_simple
#include <iostream>
#include <cstdlib>
#include <gd.h>
#include <omp.h>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <in.png> <out.png>" << endl;
		exit(1);
	}

	FILE *ifp = fopen(argv[1], "rb");
	FILE *ofp = fopen(argv[2], "wb");

	double t = omp_get_wtime();

	gdImagePtr img = gdImageCreateFromPng(ifp);
	int w = gdImageSX(img);
	int h = gdImageSY(img);

	#pragma omp parallel for schedule(guided,10)// num_threads(1/2/4/8) schedule(static/dynamic/guided)
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int color = gdImageGetPixel(img, x, y);
			int avgColor = (gdImageRed(img, color) + gdImageGreen(img, color) + gdImageBlue(img, color)) / 3;
			color = gdImageColorAllocate(img, avgColor, avgColor, avgColor);
			#pragma omp critical
			gdImageSetPixel(img, x, y, color);
		}
	}

	gdImagePng(img, ofp);
	gdImageDestroy(img);

	fclose(ifp);
	fclose(ofp);

	t = omp_get_wtime() - t;
	cout << "Time: " << t << endl;
}

//5 _ msg
#include <iostream>
#include <cstring>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) cout << "Run with: mpirun -np 4 ./5\n";
        MPI_Finalize();
        return 0;
    }

    const char *m[] = {"", "HELLO", "CSE", "RVCE"};
    char msg[32];

    if (rank != 0) {
        strcpy(msg, m[rank]);
        MPI_Send(msg, 32, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < 4; i++) {
            MPI_Recv(msg, 32, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "Received " << msg << " in process 0 from process " << i << endl;
        }
    }

    MPI_Finalize();
}

//6 _ word 
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <omp.h>
using namespace std;

void toLower(string &s) {
    for (char &c : s) c = tolower(c);
}

int getCount(string fname, string key) {
    ifstream file(fname);
    string word;
    int count = 0;
    toLower(key);
    while (file >> word) {
        toLower(word);
        if (word == key) count++;
    }
    return count;
}

int main() {
    string fname;
    cout << "Enter file name: ";
    cin >> fname;

    string words[] = {"lorem", "ipsum", "dolo", "sit", "amet"};
    int n = 5, counts[5];

    cout << "Threads\tTime(s)\n";
    int threads[] = {1, 2, 4, 8};
    for (int th : threads) {
        double t = omp_get_wtime();

        #pragma omp parallel for num_threads(th)
        for (int i = 0; i < n; i++)
            counts[i] = getCount(fname, words[i]);

        cout << th << "\t" << omp_get_wtime() - t << endl;
    }

    cout << "\nWord Counts:\n";
    for (int i = 0; i < n; i++)
        cout << words[i] << ": " << counts[i] << endl;
}


//7a - jacobi 
#include <iostream>
#include <cmath>
#include <ctime>
#include <openacc.h>
using namespace std;

#define N 1000
double u[N][N], unew[N][N], f[N][N];

int main() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            u[i][j] = 0.0;
            f[i][j] = 1.0;
        }

    int iter = 0, max_iter = 1000;
    double err = 1.0, tol = 1e-6;
    clock_t st = clock();

    #pragma acc data copyin(f[0:N][0:N]) copy(u[0:N][0:N]) create(unew[0:N][0:N])
    while (err > tol && iter < max_iter) {
        err = 0.0;

        #pragma acc parallel loop collapse(2) reduction(max:err)
        for (int i = 1; i < N - 1; i++)
            for (int j = 1; j < N - 1; j++) {
                unew[i][j] = 0.25 * (u[i-1][j] + u[i+1][j] + u[i][j-1] + u[i][j+1] - f[i][j]);
                err = fmax(err, fabs(unew[i][j] - u[i][j]));
            }

        #pragma acc parallel loop collapse(2)
        for (int i = 1; i < N - 1; i++)
            for (int j = 1; j < N - 1; j++)
                u[i][j] = unew[i][j];

        iter++;
    }

    double t = (double)(clock() - st) / CLOCKS_PER_SEC;
    cout << "Iterations: " << iter << "\nFinal Error: " << err << "\nTime: " << t << " s\n";
}


//7b - mat mul
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <openacc.h>
using namespace std;

int main() {
    int n;
    cout << "Enter matrix size: ";
    cin >> n;

    float *A = new float[n * n];
    float *B = new float[n * n];
    float *C = new float[n * n];

    for (int i = 0; i < n * n; i++) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    clock_t st = clock();

    #pragma acc data copyin(A[0:n*n], B[0:n*n]) copyout(C[0:n*n])
    {
        #pragma acc parallel loop collapse(2)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                float sum = 0;
                for (int k = 0; k < n; k++)
                    sum += A[i * n + k] * B[k * n + j];
                C[i * n + j] = sum;
            }
        }
    }

    double t = (double)(clock() - st) / CLOCKS_PER_SEC;
    cout << "Time: " << t << " s\n";

    if (n <= 4) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) cout << C[i * n + j] << " ";
            cout << endl;
        }
    }

    delete[] A;
    delete[] B;
    delete[] C;
}
