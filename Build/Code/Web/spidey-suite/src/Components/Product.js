import React from 'react';
import '../css/product.css';
import temp from '../assets/temp-image.png';

import SPIDEYDUDE from '../assets/spideydude.png';
import SPIDEYAVRDUDE from '../assets/spideyAVRdude.png';

class Product extends React.Component{
    render(){
        return(
            <>
                <section className="productSection" id="product">
                    <div className="sec_container">
                        <div className="sec-title text-center">
                            <span className="title">Offerings</span>
                            <h2>Our Products</h2>
                        </div>
                        <div className="ProjMainSection">
                            <div className="prod_container" id="spideyboot">
                                <div className="row">
                                    <div className="col-md-6 p-image-col">
                                        <div className="p-image-wrap">
                                            <img src={temp} alt="Product spideyboot"/>
                                        </div>
                                    </div>
                                    <div className="col-md-6 p-desc-col">
                                        <div className="p-dec-wrapper">
                                            <h2>spideyboot</h2>
                                            <p></p>
                                            <h4>
                                                Small and fast bootloader for ATMEL AVR chip supporting Over The Air (OTA) programming.
                                            </h4>
                                            <p className="pdesc-inst-cont">
                                                A perfect bootloader having OTA programming functionality. For the boards not having built-in WiFi <strong>spideydude</strong> programmer is required.
                                                For the boards having built-in WiFi, accordingly the program needs to be written.
                                            </p>
                                            <p></p>
                                            <div className="action-wrap product-knowM-but-cont">
                                                <a href="https://github.com/spider-tronix/spidey-suite/tree/master/spideyboot#spideyboot"><button className="get_started_but">Know More <i className="fas fa-external-link-alt"></i></button></a>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div className="prod_container" id="spideydude">
                                <div className="row">
                                    <div className="col-md-6 p-image-col p-order-rev">
                                        <div className="p-image-wrap">
                                            <img src={SPIDEYDUDE} alt="Product spideydude"/>
                                        </div>
                                    </div>
                                    <div className="col-md-6 p-desc-col">
                                        <div className="p-dec-wrapper">
                                            <h2>spideydude</h2>
                                            <p></p>
                                            <h4>
                                                A fast programmer for the microcontrollers having spideyboot bootloader. Built-in easy to use web interface to upload HEX files.  
                                            </h4>
                                            <p className="pdesc-inst-cont">
                                                Runs on ESP32 module. Can be operated in WiFi Station mode or WiFi Access Point (AP) mode. Easy configuration settings.
                                            </p>
                                            <p></p>
                                            <div className="action-wrap product-knowM-but-cont">
                                                <a href="https://github.com/spider-tronix/spidey-suite/tree/master/spideydude#spideydude"><button className="get_started_but">Know More <i className="fas fa-external-link-alt"></i></button></a>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div className="prod_container" id="spideyAVRdude">
                                <div className="row">
                                    <div className="col-md-6 p-image-col">
                                        <div className="p-image-wrap">
                                            <img src={SPIDEYAVRDUDE} alt="Product spideyboot"/>
                                        </div>
                                    </div>
                                    <div className="col-md-6 p-desc-col">
                                        <div className="p-dec-wrapper">
                                            <h2>spideyAVRdude</h2>
                                            <p></p>
                                            <h4>
                                                A fast OTA avrdude programmer for Arduino's Optiboot bootloader. Built-in easy to use web interface to upload HEX files.
                                            </h4>
                                            <p className="pdesc-inst-cont">
                                                OTA programer for the Arduino boards not having built-in WiFi and having Optiboot bootloader in it. Can be operated in WiFi Station mode or WiFi Access Point (AP) mode. Easy configuration settings.
                                            </p>
                                            <p></p>
                                            <div className="action-wrap product-knowM-but-cont">
                                                <a href="https://github.com/spider-tronix/spidey-suite/tree/master/spideyAVRdude#spideyavrdude"><button className="get_started_but">Know More <i className="fas fa-external-link-alt"></i></button></a>
                                            </div>
                                        </div>
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

export default Product;
