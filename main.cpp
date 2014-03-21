#include "bucket.h"
#include "hashingMethod.h"

int main()
{
    std::ofstream ofs("bucket_test.txt");
    Bucket bucket;
    bucket.putValue("0");
    bucket.putValue("1");
    bucket.putValue("2");

    std::cout << bucket << std::endl;
    {
        boost::archive::text_oarchive oa(ofs);
        oa << bucket;
    }

    Bucket new_bucket;
    {
        std::ifstream ifs("bucket_test.txt");
        boost::archive::text_iarchive ia(ifs);
        ia >> new_bucket;
    }
    std::cout << new_bucket << std::endl;
    return 0;
}
