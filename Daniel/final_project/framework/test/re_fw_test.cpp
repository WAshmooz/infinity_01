#include "nbd_driver_communicator.hpp"
#include "i_storage.hpp"
#include "ram_storage.hpp"
#include "driver_data.hpp"

#include "input_src_imp_for_test.hpp"
#include "re_fw.hpp"

/*------------------------------------------------------------------------------
                                main
------------------------------------------------------------------------------*/
int test1()
{
    std::cout << "test1 Started" << std::endl;

    std::string path = "/dev/nbd1";
    size_t storage_size = 5000 * 4096;

    // Create an instance of NBDriverCommunicator object 
    hrd31::NBDriverCommunicator nbd_driv_coomm(path, storage_size);
    hrd31::RAMStorage ram_storage(storage_size);

    // Create an instance of InputSrcNBD
    // hrd31::InputSrcNBD input_src_nbd(nbd_driv_coomm, ram_storage);

    // Create an instance of RequestEngine
    hrd31::RequestEngine<hrd31::req_type_t, std::shared_ptr<hrd31::IArgs>> request_engine;

    // Configure the InputSrcNBD in the RequestEngine 
    request_engine.ConfigInputSrc(std::make_shared<hrd31::InputSrcNBD>(nbd_driv_coomm, ram_storage));


    request_engine.ConfigTask(hrd31::req_type_t::READ, &hrd31::NBDTask::Create);
    request_engine.ConfigTask(hrd31::req_type_t::WRITE, &hrd31::NBDTask::Create);


    // Configure the InputSrcStdin in the RequestEngine 
    request_engine.ConfigInputSrc(std::make_shared<hrd31::InputSrcStdin>(request_engine, nbd_driv_coomm));


    request_engine.ConfigTask(hrd31::req_type_t::STOP, &hrd31::StinTask::Create);

    // Run the reacto
    request_engine.Run();

    return 0;
}


int main()
{
    test1();
    return 0;
}

