( function() {
    // Iframe auto resize
    function setSize() {
        let docHeight = window.innerHeight;

        let barHeight = document.getElementById( 'navbar' ).clientHeight;
        let footerHeight = document.querySelector( '.footer-copyright' ).clientHeight;

        let iframe = document.querySelector( 'iframe' );
        iframe.height = docHeight - footerHeight - barHeight;
    }
    setSize();
    window.onresize = setSize;
}() );
