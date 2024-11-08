/*
 * https://www.gnu.org/software/gsl/manual/html_node/Fitting-Examples-for-multi_002dparameter-linear-regression.html#Fitting-Examples-for-multi_002dparameter-linear-regression

Permission is granted to copy, distribute and/or modify this document
under the terms of the GNU Free Documentation License, Version 1.3 or
any later version published by the Free Software Foundation; with the
Invariant Sections being "GNU General Public License" and "Free Software
Needs Free Documentation", the Front-Cover text being "A GNU Manual",
and with the Back-Cover Text being (a) (see below). A copy of the
license is included in the section entitled "GNU Free Documentation
License".
 *
 */

#ifdef MULTIFIT_PGRM

/*
<p>The following program performs a quadratic fit <em>y = c_0 + c_1 x + c_2
x^2</em> to a weighted dataset using the generalised linear fitting function
<code>gsl_multifit_wlinear</code>.  The model matrix <em>X</em> for a quadratic
fit is given by,
</p>
<div class="example">
<pre class="example">X = [ 1   , x_0  , x_0^2 ;
      1   , x_1  , x_1^2 ;
      1   , x_2  , x_2^2 ;
      ... , ...  , ...   ]
</pre></div>

<p>where the column of ones corresponds to the constant term <em>c_0</em>.
The two remaining columns corresponds to the terms <em>c_1 x</em> and
<em>c_2 x^2</em>.
</p>
<p>The program reads <var>n</var> lines of data in the format (<var>x</var>, <var>y</var>,
<var>err</var>) where <var>err</var> is the error (standard deviation) in the
value <var>y</var>.
</p>
*/

#include <stdio.h>
#include <gsl/gsl_multifit.h>

int
main (int argc, char **argv)
{
  int i, n;
  double xi, yi, ei, chisq;
  gsl_matrix *X, *cov;
  gsl_vector *y, *w, *c, *r, *e, *x;

  if (argc != 2)
    {
      fprintf (stderr,"usage: fit n < data\n");
      exit (-1);
    }

  n = atoi (argv[1]);

  X = gsl_matrix_alloc (n, 3);
  y = gsl_vector_alloc (n);
  w = gsl_vector_alloc (n);

  r = gsl_vector_alloc (n);
  e = gsl_vector_alloc (n);
  x = gsl_vector_alloc (n);

  c = gsl_vector_alloc (3);
  cov = gsl_matrix_alloc (3, 3);

  for (i = 0; i < n; i++)
    {
      int count = fscanf (stdin, "%lg %lg %lg",
                          &xi, &yi, &ei);

      if (count != 3)
        {
          fprintf (stderr, "error reading file\n");
          exit (-1);
        }

      printf ("%g %g +/- %g\n", xi, yi, ei);
      
      gsl_matrix_set (X, i, 0, 1.0);
      gsl_matrix_set (X, i, 1, xi);
      gsl_matrix_set (X, i, 2, xi*xi);
      
      gsl_vector_set (y, i, yi);
      gsl_vector_set (w, i, 1.0/(ei*ei));

      gsl_vector_set (e, i, ei);
      gsl_vector_set (x, i, xi);
    }

  {
    gsl_multifit_linear_workspace * work 
      = gsl_multifit_linear_alloc (n, 3);
    gsl_multifit_wlinear (X, w, y, c, cov,
                          &chisq, work);
    gsl_multifit_linear_free (work);

    gsl_multifit_linear_residuals (X, y, c, r);
  }

  for (i = 0; i < n; i++)
    {
        printf ("%g %g +/- %g %g #resid\n", gsl_vector_get(x,i),
            gsl_vector_get(y,i), gsl_vector_get(e,i), gsl_vector_get(r,i));
    }

#define C(i) (gsl_vector_get(c,(i)))
#define COV(i,j) (gsl_matrix_get(cov,(i),(j)))

  {
    printf ("# best fit: Y = %g + %g X + %g X^2\n", 
            C(0), C(1), C(2));

    printf ("# covariance matrix:\n");
    printf ("[ %+.5e, %+.5e, %+.5e  \n",
               COV(0,0), COV(0,1), COV(0,2));
    printf ("  %+.5e, %+.5e, %+.5e  \n", 
               COV(1,0), COV(1,1), COV(1,2));
    printf ("  %+.5e, %+.5e, %+.5e ]\n", 
               COV(2,0), COV(2,1), COV(2,2));
    printf ("# chisq = %g\n", chisq);
  }

  gsl_matrix_free (X);
  gsl_vector_free (y);
  gsl_vector_free (w);
  gsl_vector_free (c);
  gsl_matrix_free (cov);

  gsl_vector_free (r);
  gsl_vector_free (e);
  gsl_vector_free (x);
  return 0;
}

#endif /* MULTIFIT_PGRM */

#ifdef MULTIFIT_DATA 
/*
<p>A suitable set of data for fitting can be generated using the following
program.  It outputs a set of points with gaussian errors from the curve
<em>y = e^x</em> in the region <em>0 &lt; x &lt; 2</em>.
</p>
*/
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_randist.h>

int
main (void)
{
  double x;
  const gsl_rng_type * T;
  gsl_rng * r;
  
  gsl_rng_env_setup ();
  
  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  for (x = 0.1; x < 2; x+= 0.1)
    {
      double y0 = exp (x);
      double sigma = 0.1 * y0;
      double dy = gsl_ran_gaussian (r, sigma);

      printf ("%g %g %g\n", x, y0 + dy, sigma);
    }

  gsl_rng_free(r);

  return 0;
}

/*

<p>The data can be prepared by running the resulting executable program,
</p>
<div class="example">
<pre class="example">$ GSL_RNG_TYPE=mt19937_1999 ./generate &gt; exp.dat
$ more exp.dat
0.1 0.97935 0.110517
0.2 1.3359 0.12214
0.3 1.52573 0.134986
0.4 1.60318 0.149182
0.5 1.81731 0.164872
0.6 1.92475 0.182212
....
</pre></div>

<p>To fit the data use the previous program, with the number of data points
given as the first argument.  In this case there are 19 data points.
</p>
<div class="example">
<pre class="example">$ ./fit 19 &lt; exp.dat
0.1 0.97935 +/- 0.110517
0.2 1.3359 +/- 0.12214
...
# best fit: Y = 1.02318 + 0.956201 X + 0.876796 X^2
# covariance matrix:
[ +1.25612e-02, -3.64387e-02, +1.94389e-02  
  -3.64387e-02, +1.42339e-01, -8.48761e-02  
  +1.94389e-02, -8.48761e-02, +5.60243e-02 ]
# chisq = 23.0987
</pre></div>

<p>The parameters of the quadratic fit match the coefficients of the
expansion of <em>e^x</em>, taking into account the errors on the
parameters and the <em>O(x^3)</em> difference between the exponential and
quadratic functions for the larger values of <em>x</em>.  The errors on
the parameters are given by the square-root of the corresponding
diagonal elements of the covariance matrix.  The chi-squared per degree
of freedom is 1.4, indicating a reasonable fit to the data.
</p>

*/

#endif /* MULTIFIT_DATA */

/*
 * eof
 */
