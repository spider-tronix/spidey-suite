import React from 'react';
import '../css/product.css';
import temp from '../assets/temp-image.png';

class Product extends React.Component{
    render(){
        return(
            <>
                <section className="productSection" id="product">
                    <div className="sec_container">
                        <div className="sec-title text-center">
                            <span className="title">Offerings</span>
                            <h2>Products</h2>
                        </div>
                        <div className="ProjMainSection">
                            <div className="prod_container">
                                <div className="row">
                                    <div className="col-md-5 p-image-col">
                                        <div className="p-image-wrap">
                                            <img src={temp} alt="Product spideyboot"/>
                                        </div>
                                    </div>
                                    <div className="col-md-7 p-desc-col">
                                        <div className="p-dec-wrapper">
                                            <h2>spideyboot</h2>
                                            <p></p>
                                            <h4>
                                                Small and fast bootloader for ATMEL AVR chip supporting Over The Air (OTA) programming. The bootloader currently supports ATMEGA328p.
                                            </h4>
                                            <p className="pdesc-inst-cont">
                                                A perfect bootloader having OTA functionality. For the boards not having built-in WiFi <strong>spideydude</strong> programmer is required.
                                                For the boards having built-in WiFi, accordingly the program needs to be written.
                                            </p>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div className="prod_container">
                                <div className="row">
                                    <div className="col-md-5 p-image-col p-order-rev">
                                        <div className="p-image-wrap">
                                            <img src={temp} alt="Product spideydude"/>
                                        </div>
                                    </div>
                                    <div className="col-md-7 p-desc-col">
                                        <div className="p-dec-wrapper">
                                            <h2>spideydude</h2>
                                            <p></p>
                                            <h4>
                                                Fast programmer for OTA programming chips having spideyboot bootloader. Built-in easy to use web interface to upload HEX files.  
                                            </h4>
                                            <p className="pdesc-inst-cont">
                                                Runs on ESP32 module. Can be operated in WiFi Station mode or WiFi Access Point (AP) mode. Easy to configure settings.
                                            </p>
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
