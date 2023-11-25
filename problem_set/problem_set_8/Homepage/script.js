// When the user scrolls down 20px from the top of the document, show the button
function scrollFunction(button) {
    let limit = 30;
    if (document.body.scrollTop > limit || document.documentElement.scrollTop > limit) {
        button.style.display = "block";
    } else {
        button.style.display = "none";
    }
}

// When the user clicks on the button, scroll to the top of the document
function topFunction() {
    // For Safari
    document.body.scrollTop = 0;

    // For Chrome, Firefox, IE and Opera
    document.documentElement.scrollTop = 0;
}

// Wait until DOM is loaded
window.addEventListener('DOMContentLoaded', function(event) {
    let button = document.getElementById("goTop");
    // Show button on scroll down
    window.onscroll = function() {scrollFunction(button)};
});
