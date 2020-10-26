import React from 'react';
import Lander from '../Components/Lander.js';
import About from '../Components/About.js';
import Collab from '../Components/Collaborate.js';
import Footer from '../Components/Footer.js';

class Home extends React.Component{
    render(){
        return(
            <>
                <Lander/>
                <Collab/>
                <Footer/>
            </>
        );
    }
}

export default Home;
