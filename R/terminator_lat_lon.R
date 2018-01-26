#' Generate a full set of terminator frames
#'
#' @param day,from,to,by setup for the hours sequence
#' @export
terminator_lat_lon <- function(day = Sys.Date(), from=0, to=23, by=1) {

  purrr::map_df(seq(0, 23, 1), ~{
    out <- terminator(as.integer((as.POSIXct(day) + (60*60*.x))), -180, 190, 0.5)
    out$frame <- .x
    out
  })

}