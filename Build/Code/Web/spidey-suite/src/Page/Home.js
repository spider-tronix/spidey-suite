import React from 'react';
import Lander from '../Components/Lander.js';
import Feature from '../Components/Feature.js';
import Product from '../Components/Product.js';
import Collab from '../Components/Collaborate.js';
import Footer from '../Components/Footer.js';

class Home extends React.Component{
    render(){
        return(
            <>
                <Lander/>
                <Feature/>
                <Product/>
                <Collab/>
                <Footer/>
            </>
        );
    }
}

export default Home;
