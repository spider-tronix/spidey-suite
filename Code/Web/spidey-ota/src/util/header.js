function scrollSpy(){
    if((document.body.scrollTop || document.documentElement.scrollTop) > (window.innerHeight - 50)){
        document.getElementById("header").classList.add('noTop');
    }
    else{
        document.getElementById("header").classList.remove('noTop');
    }
}
scrollSpy();
window.addEventListener("scroll",scrollSpy);