import React from 'react';
import '../css/landing.css';
import LanderImage from '../assets/LanderImage.png';

class Lander extends React.Component{
    render(){
        return(
            <>
                <section className="secFitHeight" id="home">
                    <div className="layerWrapper">
                        <div className="layerbgcont"></div>
                    </div>
                    <div className="sec_container">
                        <div className="ldg_wrapper container-fluid ">
                            <div className="row">
                                <div className="col-lg-6 col-md-12 col-sm-12 order-2">
                                    <div className="header-text-area">
                                        <h1>Add OTA update feature in minutes</h1>
                                        <h3>Working on adding OTA (Over The Air) functionality to your microcontroller and looking for a compatible bootloader? <b>spideyboot</b> and <b>spideydude</b> can come in handy.</h3>
                                        <div className="action-wrap">
                                            <a href="#about"><button className="get_started_but">Explore More</button></a>
                                        </div>
                                    </div>
                                </div>
                                <div className="col-lg-6 col-md-12 col-sm-12">
                                    <div className="ldg_img_box">
                                        <img src={LanderImage} alt="Landing Laptop"/>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </section>
            </>
        );
    }
}

export default Lander;