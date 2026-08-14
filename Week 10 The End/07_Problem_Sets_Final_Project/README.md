# Python Travel
#### Description: 
Abstract: In this project, a database was created for a public transport agency to store information about bus and train routes, timetables and fare prices. ER model, relational model and algebra, SQL language, normalization are used. Written in Python and SQL environment.

Introduction: A database-management system (DBMS) is a collection of interrelated data and a set of programs to access those data. The collection of data, usually referred to as the database, contains information relevant to an enterprise. The primary goal of a DBMS is to provide a way to store and retrieve database information that is both convenient and efficient. Database systems are designed to manage large bodies of information. Management of data involves both defining structures for storage of information and providing mechanisms for the manipulation of information. In addition, the database system must ensure the safety of the information stored, despite system crashes or attempts at unauthorized access. If data are to be shared among several users, the system must avoid possible anomalous results. Because information is so important in most organizations, computer scientists have developed a large body of concepts and techniques for managing data. These concepts and techniques form the focus of this book. This chapter briefly introduces the principles of database systems. 

Purpose of Public Transport Company Database: The purpose of these database systems is to create a database where company officials can add, update and delete new vehicles, routes, and users can inquire the tickets they have purchased, where they can search routes and view active expedition. The essence of the job, this is a ticket selling application for travelling companies. If their company grows and if they want to add new expedition to their route this web-app is usefull for this. Also if they buy a new bus or change a old bus they can delete the old one and add the new one to their systems. On the other hand the customers can buy the tickets for their own object. They can easily query the tickets with just starting point and end point. After that the available tickets is loaded to the page and customer can buy the tickets by looking to date and vehicle type etc. After bought the tickets customers can see their own tickets with their name, TC no and phone number. If an unplanned situation occurs and they no more need that ticket, they can easily cancel the ticket. Programmers develop these application programs to meet the needs of the company and users. 

Structure of Relational Databases: 
Route(route_id, starting_station, destination, date, time_of_journey, price, vehicle_id(FK)) 
Vehicle(vehicle_id, vehicle_type, passenger_capacity, route_id(FK)) 
Ticket(ticket_id, customer_name, tc_no, vehicle_id(FK), route_id(FK), phone) 
Admin(id, User_name, password)



Database Users and User Interfaces: There are two different types of database system users that differ in the way they hope to interact with the system. Different user interfaces are designed for different types of users. Thanks to the web application, admin users can add vehicles with the privileges granted to them, add expeditions, view the tickets for sale and make changes on them. Standard users, on the other hand, can enter the departure route and arrival route and inquire about company expedition. In addition, the user can query the tickets he has previously bought by entering his own name, tc number and phone number.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**

