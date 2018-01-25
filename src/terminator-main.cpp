#define _USE_MATH_DEFINES

#include <Rcpp.h>
#include <math.h>

using namespace Rcpp;

#define rad2deg(rad) ((rad * 180.0) / M_PI)
#define deg2rad(deg) ((deg * M_PI) / 180.0)
#define get_julian(time) (((double)time / 86400.0) + 2440587.5)
#define get_gmst(j_day) (fmod((18.697374558 + 24.06570982441908 * (j_day - 2451545.0)), 24.0))

inline std::vector < double > sun_ecliptic_position(double j_day) {

  // compute the position of the Sun in ecliptic coordinates days since start of J2000.0
  double n = j_day - 2451545.0;

  // mean longitude of the Sun
  double L = fmod((280.460 + 0.9856474 * n), 360.0);

  // mean anomaly of the Sun
  double g = fmod((357.528 + 0.9856003 * n), 360.0);

  // ecliptic longitude of Sun
  double lambda = L + 1.915 * sin(deg2rad(g)) + 0.02 * sin(2 * deg2rad(g));

  // distance from Sun in AU
  double R = 1.00014 - 0.01671 * cos(deg2rad(g)) - 0.0014 * cos(2 * deg2rad(g));

  std::vector< double > ret(2);
  ret[0] = lambda;
  ret[1] = R;

  return(ret);

}

inline double ecliptic_obliquity(double j_day) {

  double n = j_day - 2451545.0;

  // Julian centuries since J2000.0
  double T = n / 36525.0;

  // compute epsilon
  return(23.43929111 -
    T * (46.836769 / 3600.0
         - T * (0.0001831 / 3600.0
                + T * (0.00200340 / 3600.0
                       - T * (0.576e-6 / 3600.0
                              - T * 4.34e-8 / 3600.0)))));

}

std::vector< double > sun_equatorial_position(double lng, double obliq) {

  double alpha = rad2deg(atan(cos(deg2rad(obliq)) * tan(deg2rad(lng))));
  double delta = rad2deg(asin(sin(deg2rad(obliq)) * sin(deg2rad(lng))));

  double lQuadrant  = floor(lng / 90.0) * 90.0;
  double raQuadrant = floor(alpha / 90.0) * 90.0;

  alpha = alpha + (lQuadrant - raQuadrant);

  std::vector< double > ret(2);

  ret[0] = alpha;
  ret[1] = delta;

  return(ret);

}

inline double hour_angle(double lng, std::vector< double >sun_pos, double gst) {
  return((gst + lng / 15.0) * 15.0 - sun_pos[0]);
}

inline double longitude(double ha, std::vector< double >sun_pos) {
  return(rad2deg(atan(-cos(deg2rad(ha)) / tan(deg2rad(sun_pos[1])))));
}

//' Compute a single termiantor band
//'
//' Returns a dataframe of latitude and longitude for the line that separates illuminated
//' day and dark night for any given time
//'
//' @md
//' @param time time (numeric from `POSIXct`) for the computation (bands are time-dependent)
//' @param from,to,by latitude sequence setup
//' @return data frame
//' @references <https://github.com/joergdietrich/Leaflet.Terminator/blob/master/L.Terminator.js>,
//'     <https://github.com/JoGall/terminator/blob/master/terminator.R>
//' @export
// [[Rcpp::export]]
DataFrame terminator(int time, double from = -180, double to = 180, double by = 0.1) {

  // calculate latitude and longitude of terminator within specified range using time (in POSIXct format, e.g. `Sys.time()`)
  double j_day = get_julian(time);

	double gst = get_gmst(j_day);

	std::vector< double > sunEclPos = sun_ecliptic_position(j_day);

  double eclObliq = ecliptic_obliquity(j_day);

	std::vector< double > sunEqPos = sun_equatorial_position(sunEclPos[0], eclObliq);

	std::vector< double > out_lat, out_lon;

	out_lat.reserve(4000);
	out_lon.reserve(4000);

	int n=0;

	for (double i=from; i<=to; i+=by) {
	  n += 1;
	  out_lat.push_back(i);
    out_lon.push_back(
      longitude(
        hour_angle(i, sunEqPos, gst),
        sunEqPos
      )
    );
	}

	out_lat.resize(n);
	out_lon.resize(n);

	return(
	  DataFrame::create(
	    Named("lat") = out_lat,
	    Named("lon") = out_lon
	  )
	);

}