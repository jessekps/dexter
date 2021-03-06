% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/tia.R
\name{tia_tables}
\alias{tia_tables}
\title{Simple test-item analysis}
\usage{
tia_tables(
  dataSrc,
  predicate = NULL,
  type = c("raw", "averaged", "compared"),
  max_scores = c("observed", "theoretical")
)
}
\arguments{
\item{dataSrc}{a connection to a dexter database, a matrix, or a data.frame with columns: person_id, item_id, item_score}

\item{predicate}{An optional expression to subset data, if NULL all data is used}

\item{type}{How to present the item level statistics: \code{raw} for each test booklet 
separately, \code{averaged} booklets are ignored, with the exception of rit and rir which are averaged over the test booklets,
with the number of persons as weights, or \code{compared}, in which case the pvalues, 
correlations with the sum score (rit), and correlations with the rest score (rit) are 
shown in separate tables and compared across booklets}

\item{max_scores}{use the observed maximum item score or the theoretical maximum item score 
according to the scoring rules in the database to compute pvalues and maximum scores}
}
\value{
A list containing:
\item{booklets}{a data.frame of statistics at booklet level} 
\item{items}{a data.frame (or list if type='compared') of statistics at item level}
}
\description{
Show simple Classical Test Analysis statistics
at item and test level
}
\details{
The returned list also contains the elements 'testStats' and 'itemStats'. These contain the same information
as 'booklets' and 'items' but using obsolete (mixed camelCase and snake_case) variable names. These will
be removed in the future and users are advised to not use these in newer code.
}
