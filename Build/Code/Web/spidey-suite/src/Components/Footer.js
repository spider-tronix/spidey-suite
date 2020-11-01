import React from 'react';
import '../css/footer.css';

class Footer extends React.Component{
    render(){
        return(
            <>
                <footer className="main-footer" id="contact">
                    <div className="sec_container">
                        <div className="footerMainSec">
                            <div className="row">
                                <div className="col-xl-3 col-lg-3 col-md-6 col-sm-12">
                                    <div className="footerWidget">
                                        <h1 className="widget-title footerTitle">Spidey Suite</h1>
                                        <div className="widget-container">
                                            <div className="text">Spider Tronix, <br/>Spider R&D Lab, Ground floor, Old SAC, (Opposite to CCD), <br/>NIT Tiruchirappalli.</div>
                                        </div>
                                    </div>
                                </div>
                                <div className="col-xl-3 col-lg-3 col-md-6 col-sm-12">
                                    <div className="footerWidget">
                                        <h2 className="widget-title">Quick Links</h2>
                                        <div className="widget-container">
                                            <ul className="list">
                                                <li><a href="https://spider-tronix.github.io">Spider Tronix</a></li>
                                                <li><a href="https://spider.nitt.edu">Spider R&D Club</a></li>  
                                            </ul>
                                        </div>
                                    </div>
                                </div>
                                <div className="col-xl-3 col-lg-3 col-md-6 col-sm-12">
                                    <div className="footerWidget">
                                        <h2 className="widget-title">Downloads</h2>
                                        <div className="widget-container">
                                            <ul className="list">
                                                <li><a href="#">spideydude</a></li>
                                                <li><a href="#">spideyboot</a></li>  
                                            </ul>
                                        </div>
                                    </div>
                                </div>
                                <div className="col-xl-3 col-lg-3 col-xl-3 col-lg-3 col-md-6 col-sm-12">
                                    <div className="footerWidget">
                                        <h2 className="widget-title">Contact Us</h2>
                                        <div className="widget-container">
                                            <ul className="list">
                                                <li><a href="mailto:nitt.spider@gmail.com">nitt.spider@gmail.com</a></li>
                                                <li><a href="#">(+91) 9470186194</a></li>  
                                            </ul>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div className="footer-bottom">
                        <div className="sec_container">
                            <div className="outer-box">
                                <div className="copyright-text">© 2020 All Rights Reserved</div>
                                <ul className="privacy">
                                    <li className="menu-item">Made with <a title="Privacy Policy" href="https://fontawesome.com/">Font Awesome</a> and loads of <label style={{"color": "red","marginLeft":"4px"}}>❤</label></li>
                                </ul>
                            </div>
                        </div>
                    </div>
                </footer>
            </>
        );
    }
}

export default Footer;