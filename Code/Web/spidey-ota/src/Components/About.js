import React from 'react';
import '../css/about.css';

class About extends React.Component{
    render(){
        return(
            <>
                <section className="aboutSec" id="about">
                    <div className="layer-image"></div> 
                    <div className="autoContainer">
                        <div className="sec-title text-center">
                            <span className="title">ABOUT</span>
                            <h2>How it Works?</h2>
                        </div>
                    </div>
                </section>
            </>
        );
    }
}

export default About;