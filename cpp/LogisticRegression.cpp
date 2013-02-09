#include <iostream>
#include <string>
#include <math.h>
#include "LogisticRegression.h"
using namespace std;


LogisticRegression::LogisticRegression(int size, int in, int out) {
  N = size;
  n_in = in;
  n_out = out;

  // initialize W, b
  W = new double*[n_in];
  for (int i=0; i<n_in; i++) W[i] = new double[n_out];
  b = new double[n_out];
}

LogisticRegression::~LogisticRegression() {
  for (int i=0; i<n_out; i++) delete[] W[i];
  delete[] W;
  delete[] b;
}


void LogisticRegression::train(int *x, int *y, double lr) {
  int i,j;
  double p_y_given_x[n_out];
  double dy[n_out];

  for (i=0; i<n_out; i++) {
    for (j=0; j<n_in; j++) {
      p_y_given_x[i] += W[i][j] * x[j];
    }
    p_y_given_x[i] += b[i];
  }
  softmax(p_y_given_x);

  for (i=0; i<n_out; i++) {
    dy[i] = y[i] - p_y_given_x[i];

    for (j=0; j<n_in; j++) {
      W[i][j] += lr * dy[i] * x[j] / N;
    }

    b[i] += lr * dy[i] / N;
  }
}

void LogisticRegression::softmax(double *x) {
  double max;
  double sum;
  
  int i;
  for (i=0; i<n_out; i++) if(max < x[i]) max = x[i];
  for (i=0; i<n_out; i++) {
    x[i] = exp(x[i] - max);
    sum += x[i];
  } 

  for(i=0; i<n_out; i++) x[i] /= sum;
}

void LogisticRegression::predict(int *x, double *y) {
  for (int i=0; i<n_out; i++) {
    for (int j=0; j<n_in; j++) {
      y[i] += W[i][j] * x[j];
    }
    y[i] += b[i];
  }

  softmax(y);
}


void test_lr() {
  int i,j;

  double learning_rate = 0.1;
  double n_epochs = 500;

  int train_N = 6;
  int test_N = 1;
  int n_in = 6;
  int n_out = 2;
  // int **train_X;
  // int **train_Y;
  // int **test_X;
  // double **test_Y;

  // train_X = new int*[train_N];
  // train_Y = new int*[train_N];
  // for (i=0; i<train_N; i++){
  //   train_X[i] = new int[n_in];
  //   train_Y[i] = new int[n_out];
  // };

  // test_X = new int*[test_N];
  // test_Y = new double*[test_N];
  // for (i=0; i<test_N; i++){
  //   test_X[i] = new int[n_in];
  //   test_Y[i] = new double[n_out];
  // }


  // training data
  int train_X[6][6] = {
    {1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0}
  };

  int train_Y[6][2] = {
    {1, 0},
    {1, 0},
    {1, 0},
    {0, 1},
    {0, 1},
    {0, 1}
  };


  // construct LogisticRegression
  LogisticRegression classifier(train_N, n_in, n_out);


  // train online
  for (int epoch=0; epoch<n_epochs; epoch++) {
    for (i=0; i<train_N; i++) {
      classifier.train(train_X[i], train_Y[i], learning_rate);
    }
    learning_rate *= 0.95;
  }


  // test data
  int test_X[1][6] = {
    {1, 1, 1, 0, 0, 0}
  };

  double test_Y[1][2];


  // test
  for (i=0; i<test_N; i++) {
    classifier.predict(test_X[i], test_Y[i]);
    for (j=0; j<n_out; j++) {
      cout << test_Y[i][j] << endl;
    }
  }

}


int main() {
  test_lr();
  return 0;
}
