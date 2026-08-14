-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/1.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 1: List the titles of all movies released in 2008.
--
-- SQL logic:
--   `movies` is scanned with a WHERE equality filter on the `year` column.
--   Because `year = 2008` is an exact (non-range) predicate, the query is a
--   simple filtered projection: SELECT pulls only the `title` attribute and
--   the WHERE clause restricts the result set to 2008 releases. No joins are
--   required since every field lives in the single `movies` table.
--
-- Returns:
--   One column, `title`, containing every movie whose release year is 2008.
-- ==============================================================================

SELECT title
FROM movies
WHERE year = 2008;
