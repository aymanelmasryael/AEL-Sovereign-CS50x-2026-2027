-- Standard CS50 Complex SQL queries for IMDb database
-- Testing indexing, JOINs, and nested SELECTs

SELECT title FROM movies WHERE year = 2008;

SELECT birth FROM people WHERE name = 'Emma Stone';

SELECT title FROM movies WHERE year >= 2018 ORDER BY title;

SELECT COUNT(*) FROM ratings WHERE rating = 10.0;

SELECT title, year FROM movies WHERE title LIKE 'Harry Potter%' ORDER BY year;

SELECT AVG(rating) FROM ratings WHERE movie_id IN (SELECT id FROM movies WHERE year = 2012);

SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE movies.year = 2010 ORDER BY ratings.rating DESC, movies.title ASC;

SELECT name FROM people JOIN stars ON people.id = stars.person_id JOIN movies ON stars.movie_id = movies.id WHERE movies.title = 'Toy Story';
