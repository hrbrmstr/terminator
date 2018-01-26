#' Generate a full set of terminator frames
#'
#' @param day,hrs_from,hrs_to,hrs_by setup for the hours sequence
#' @param lon_from,lon_to,lon_by latitude sequence setup
#' @export
terminator_lat_lon <- function(day = Sys.Date(), hrs_from=0, hrs_to=23, hrs_by=1,
                               lon_from=-180, lon_to=190, lon_by=0.5) {

  purrr::map_df(seq(hrs_from, hrs_to, hrs_by), ~{
    terminator(
      as.integer((as.POSIXct(day) + (60*60*.x))),
      lon_from, lon_to, lon_by
    ) -> out
    out$frame <- .x
    out
  })

}