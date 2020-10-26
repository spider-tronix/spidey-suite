import React from 'react';
import '../css/collab.css';

class Collab extends React.Component{
    render(){
        return(
            <>
                <section className="collabSec" id="collab">
                    <div className="sec_container collabWrapper">
                        <div className="outer-box">
                            <div className="row">
                                <div className="title-column col-md-8">
                                    <h3>Contribute</h3><br/>
                                    <p>
                                        To report a problem, create an <span style={{"color":"black"}}>issue</span> clearly stating the problem. Our team will look into the problem and will fix it as soon as possible.
                                    </p>
                                    <p>
                                        To suggest changes/improvements, create a <span style={{"color":"black"}}>pull-request</span> with the modified files.
                                        Upon successfully validating the changes by one of our team, the suggested changes will be added to the repository.
                                    </p>
                                </div>                
                                <div className="link-column col-md-4">
                                    <div className="link-box text-center">
                                        <a href="https://github.com/spider-tronix/OTA_Programming_Module" className="btn-style-one" title="Visit Github Repository">Explore <i className="fas fa-external-link-alt"></i></a>
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

export default Collab;