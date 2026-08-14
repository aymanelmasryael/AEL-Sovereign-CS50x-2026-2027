-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 10
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the names of all people who have directed a
-- movie that received a rating of at least 9.0.
--
-- The directors table is the bridge between people and movies, and the
-- ratings table supplies each film's score.  Joining all four tables
-- links each director to the ratings of the films they directed; the
-- WHERE clause keeps only films scoring 9.0 or higher.  DISTINCT drops
-- duplicates so each qualifying director is listed exactly once.
-- ============================================================

SELECT DISTINCT people.name
FROM people
JOIN directors ON directors.person_id = people.id
JOIN movies ON movies.id = directors.movie_id
JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.rating >= 9.0;