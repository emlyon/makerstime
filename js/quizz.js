function shuffleArray( arr ) {
    let i, j, x;

    for( i = arr.length; i; i -- ) {
        j = Math.floor( Math.random() * i );
        x = arr[ i - 1 ];
        arr[ i - 1 ] = arr[ j ];
        arr[ j ] = x;
    }
}

// Create quizz
let quizz = document.getElementById( 'quizz' );
let quizzSet = quizzData[ quizz.dataset.cat ];
let questionBlocks = [];

quizzSet.forEach( ( q, i ) => {
    let questionBlock = document.createElement( 'div' );

    questionBlock.classList.add( 'questionBlock' );

    let question = document.createElement( 'p' );

    question.classList.add( 'question' );
    question.innerText = q.question;
    questionBlock.appendChild( question );

    let choices = [];

    q.choices.forEach( ( c, j ) => {
        let choiceBlock = document.createElement( 'div' );

        choiceBlock.classList.add( 'choiceBlock' );

        let input = document.createElement( 'input' );

        input.type = 'radio';
        input.id = `choice${ i }-${ j }`;
        input.name = `question${ i }`;
        input.classList.add( 'choice' );
        input.dataset.question = i;
        input.dataset.choice = j;
        choiceBlock.appendChild( input );

        let label = document.createElement( 'label' );

        label.setAttribute( 'for', `choice${ i }-${ j }` );
        label.classList.add( 'blue-grey-text', 'text-darken-3' );
        label.innerText = c;
        choiceBlock.appendChild( label );

        choices.push( choiceBlock );
    } );

    shuffleArray( choices );
    choices.forEach( c => {
        questionBlock.appendChild( c );
    } );
    questionBlocks.push( questionBlock );
} );

shuffleArray( questionBlocks );
questionBlocks.forEach( qb => {
    quizz.appendChild( qb );
} );

// Handle submission
let submit = document.getElementById( 'submit-quizz' );

submit.addEventListener( 'click', e => {
    e.preventDefault();

    let data = {};

    // Get answers
    document.querySelectorAll( '.choiceBlock' ).forEach( cb => {
        let c = cb.getElementsByTagName( 'input' )[ 0 ];

        if( c.checked ) {
            let numQuestion = c.dataset.question;
            let numChoice = c.dataset.choice;

            data[ `q${ numQuestion }` ] = `${ numChoice }/${ quizzSet[ numQuestion ].answer }`;

            if( !brightspace ) {
                if( numChoice != quizzSet[ numQuestion ].answer ) {
                    cb.classList.add( 'red', 'lighten-4' );
                }
                else {
                    cb.classList.add( 'teal', 'accent-2' );
                }
            }
        }
        else {
            cb.classList.remove( 'red', 'lighten-4', 'teal', 'accent-2' );
        }
    } );

    // console.log( data );

    // Submit answers if on brightspace
    if( brightspace ) {
        data.name = brightspace.name;
        data.course = brightspace.course;
        data.sheet = `quizz_${ quizz.dataset.cat }`;

        let submission = new Request( 'https://script.google.com/macros/s/AKfycbwBrvVMqflcBnDJNjAHg6y_9pooKwOPgYsvkK2fqq8-P7K0n2FB/exec', {
            method: 'POST',
            headers: new Headers( {
                'Content-Type': 'application/x-www-form-urlencoded'
            } ),
            body: Object.keys( data )
                .map( k => `${ encodeURIComponent( k ) }=${ encodeURIComponent( data[ k ] ) }` )
                .join( '&' )
        } );

        fetch( submission )
            .then( response => response.json() )
            .then( json => {
                console.log( json );

                if( json.result == 'success' ) {
                    showMessage( '<h5>Form submitted successfully!</h5><p>You can answer this quizz again by reloading the page.</p>' );
                }
                else {
                    showMessage( '<h5>There was an error!</h5><p>Please retry by reloading the page.</p>' );
                }
            } )
            .catch( e => {
                console.warn( e );
                showMessage( '<h5>There was an error!</h5><p>Please retry by reloading the page.</p>' );
            } );
    }
} );

function showMessage( msg ) {
    document.querySelector( '.card-content>form' ).innerHTML = msg;
}
