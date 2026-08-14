-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 11
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the titles of the five highest rated movies
-- (in order) that Chadwick Boseman starred in, starting with the highest
-- rated.
--
-- The chain people -> stars -> movies -> ratings navigates from the
-- actor to every film they appeared in and that film's IMDb score.  The
-- WHERE clause selects only Chadwick Boseman's credits, ORDER BY rating
-- DESC ranks them from best to worst, and LIMIT 5 keeps the top five.
-- ============================================================

SELECT movies.title
FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON people.id = stars.person_id
JOIN ratings ON ratings.movie_id = movies.id
WHERE people.name = 'Chadwick Boseman'
ORDER BY ratings.rating DESC
LIMIT 5;