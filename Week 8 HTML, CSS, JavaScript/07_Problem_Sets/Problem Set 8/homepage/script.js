document.addEventListener("DOMContentLoaded", function() {
    let headerDivs = document.querySelectorAll(".headerDiv");
    for (let i = 0; i < headerDivs.length; ++i)
    {
        headerDivs[i].addEventListener("click", function() {
            if (headerDivs[i].style.backgroundColor === "lightgreen")
            {
                headerDivs[i].style.backgroundColor = "lightgray";
            }
            else
            {
                headerDivs[i].style.backgroundColor = "lightgreen";
            }
        })
    }
});