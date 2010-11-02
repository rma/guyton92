void funct(double *xin, double *yout, double *fpwl, int size) {
  double x1 = 0, x2 = 0, y1 = 0, y2 = 0;

  for (int j = 1; (j * 2 + 2) < (size + 1); j++) {
    x1 = fpwl[j * 2 - 1 - 1];
    y1 = fpwl[j * 2 - 1];
    x2 = fpwl[j * 2 + 1 - 1];
    y2 = fpwl[j * 2 + 2 - 1];

    if (*xin >= x1 and *xin <= x2) {
      *yout = y1 + (*xin - x1) * (y2 - y1) / (x2 - x1);
      break;
    }
  }
}
