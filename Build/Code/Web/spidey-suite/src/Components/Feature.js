import React from 'react';
import '../css/feature.css';

class About extends React.Component{
    render(){
        return(
            <>
                <section className="featureSec" id="feature">
                    <div className="layer-image"></div> 
                    <div className="autoContainer">
                        <div className="sec-title text-center">
                            <span className="title">OUR FEATURES</span>
                            <h2>Why you should choose?</h2>
                        </div>
                        <div className="row">
                            <div className="feature-block col-lg-4 col-md-6 col-sm-12">
                                <div className="inner-box">
                                    <div className="icon-box">
                                        <span className="icon">
                                            <i className="fas fa-tasks"></i>
                                        </span>
                                    </div>
                                    <h4>Hassle free OTA</h4>
                                    <div className="text">Deploy OTA programming feature to your system in minutes in the easiest way possible!</div>
                                </div>
                            </div>
                            <div className="feature-block col-lg-4 col-md-6 col-sm-12">
                                <div className="inner-box">
                                    <div className="icon-box">
                                        <span className="icon">
                                            <i className="far fa-gem"></i>
                                        </span>
                                    </div>
                                    <h4>Gem Products</h4>
                                    <div className="text">Variety of products to just suit your OTA programming feature requirements! </div>
                                </div>
                            </div>
                            <div className="feature-block col-lg-4 col-md-6 col-sm-12">
                                <div className="inner-box">
                                    <div className="icon-box">
                                        <span className="icon">
                                            <i className="far fa-comments"></i>
                                        </span>
                                    </div>
                                    <h4>Easy Bug Reporting</h4>
                                    <div className="text">Report a bug or suggest improvements, just in few clicks!</div>
                                </div>
                            </div>
                            <div className="feature-block col-lg-4 col-md-6 col-sm-12">
                                <div className="inner-box">
                                    <div className="icon-box">
                                        <span className="icon">
                                            <i className="far fa-file-code"></i>
                                        </span>
                                    </div>
                                    <h4>Source Code Available</h4>
                                    <div className="text">You have the rights to use, study, change, improve, and distribute the product to anyone and for any purpose. Fully Open Source!</div>
                                </div>
                            </div>
                            <div className="feature-block col-lg-4 col-md-6 col-sm-12">
                                <div className="inner-box">
                                    <div className="icon-box">
                                        <span className="icon">
                                            <i className="far fa-bell"></i>
                                        </span>
                                    </div>
                                    <h4>Open to Contributions</h4>
                                    <div className="text">You can directly contribute to the product. After reviewing, your changes will be live!</div>
                                </div>
                            </div>
                            <div className="feature-block col-lg-4 col-md-6 col-sm-12">
                                <div className="inner-box">
                                    <div className="icon-box">
                                        <span className="icon">
                                            <i className="far fa-file-alt"></i>
                                        </span>
                                    </div>
                                    <h4>Fully Documented</h4>
                                    <div className="text">All codes are fully documented for easy understanding! </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </section>
            </>
        );
    }
}

export default About;