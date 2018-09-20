addEventListener( 'load', event => {
    event.preventDefault();

    // Iframe set src
    let iframe = document.querySelector( 'iframe' );

    [].forEach.call( document.querySelectorAll( '.lienDrop' ), a => {
        a.addEventListener( 'click', e => {
            e.preventDefault();
            iframe.src = a.dataset.to;
        } );
    } );

    // Materialize
    $( '.button-collapse' ).sideNav();
    $( '.parallax' ).parallax();
    $( 'select' ).material_select();

    Reflect.apply(
        [].forEach,
        document.querySelectorAll( '#nav-mobile>li' ),
        [ li => li.addEventListener( 'click', e => {
            $( '.drag-target' ).click();
        } ) ]
    );
} );
